/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.client;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.server.ServerCommand;
import projectcolossus.util.Log;

/**
 *
 * @author Andrea
 */
public class Client extends Thread {
   
    protected int playerIndex;
    
    protected String playerName;
    
    protected ArrayList<Listener> listeners;
    
    protected Socket socket;
    
    protected ObjectInputStream in;
    protected ObjectOutputStream out;
    
    protected Log log;
    
    public Client() {
        listeners = new ArrayList<Listener>();
        log = new Log(Client.class);
    }
    
    public ServerCommand.MMGameServerData findMatch(InetAddress serverAddress, int serverPort) throws IOException, ClassNotFoundException {
        
        socket = new Socket(serverAddress, serverPort);

        in = new ObjectInputStream(socket.getInputStream());        
        out = new ObjectOutputStream(socket.getOutputStream());     
        
        ServerCommand command = ServerCommand.receiveCommand(in);
        
        if(command.getCommand() == ServerCommand.MMCMD_GAMESERVER) {
            ServerCommand.MMGameServerData data = (ServerCommand.MMGameServerData) command.getData();
            socket.close();
            return data;
        } else {
            socket.close();
            throw new IOException("Couldn't contact the server");
        }
    }
    
    public Log getLog() { return log; }
    
    public int connect(InetAddress serverAddr, int serverPort, String playerName, Deck deck) throws IOException {     
        socket = new Socket(serverAddr, serverPort);

        in = new ObjectInputStream(socket.getInputStream());        
        out = new ObjectOutputStream(socket.getOutputStream());      

        out.writeUTF(playerName);
        Deck.write(out, deck);
        out.flush();
        
        playerIndex = in.readInt();
        
        start();
        
        return playerIndex;
    }
    
    public int getPlayerIndex() {
        return playerIndex;
    }
    
    public boolean addListener(Listener l) {
        return listeners.add(l);
    }
    
    public boolean removeListener(Listener l) {
        return listeners.remove(l);
    }
    
    public void close() {
        this.close(false);
    }
    
    public void close(boolean threaded) {
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_CLOSE, null, out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_CLOSE));
        }
        catch(IOException ex) {
            log.e(ex.getMessage());
        }
    }
    
    public void playPlanetCard(int cardIndex, int planetID) {
        this.playPlanetCard(cardIndex, planetID, false);
    }
    
    public void playPlanetCard(int cardIndex, int planetID, boolean threaded) {
        ServerCommand.PlayPlanetCardData data = new ServerCommand.PlayPlanetCardData(cardIndex, planetID);
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_PLAY_PLANET_CARD, data, out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_PLAY_PLANET_CARD));
        }
        catch(Exception ex) {
            log.e(ex.getMessage());
        }
    }
    
    public void playFreeCard(int cardIndex) {
        playFreeCard(cardIndex, false);
    }
    
    public void playFreeCard(int cardIndex, boolean threaded) {
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_PLAY_FREE_CARD, new Integer(cardIndex), out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_PLAY_FREE_CARD));
        }
        catch(Exception ex) {
            log.e(ex.getMessage());
        }        
    }
    
    public void moveUnit(int unitID, int fromPlanetID, int toPlanetID) {
        this.moveUnit(unitID, fromPlanetID, toPlanetID, false);
    }
    
    public void moveUnit(int unitID, int fromPlanetID, int toPlanetID, boolean threaded) {
        ServerCommand.MoveUnitData data = new ServerCommand.MoveUnitData(unitID, fromPlanetID, toPlanetID);
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_MOVE_UNIT, data, out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_MOVE_UNIT));
        }
        catch(Exception ex) {
            log.e(ex.getMessage());
        }
    }
    
    public void endTurn() {
        endTurn(false);
    }
    
    public void endTurn(boolean threaded) {
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_END_TURN, null, out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_END_TURN));
        }
        catch(Exception ex) {
            log.e(ex.getMessage());
        }        
    }
    
    public void requestUpdate() {
        requestUpdate(false);
    }
    
    public void requestUpdate(boolean threaded) {
        try {
            ServerCommand.sendCommand(ServerCommand.CMD_REQUEST_UPDATE, null, out, threaded);
            log.i("Sent: " + ServerCommand.getCommandDescription(ServerCommand.CMD_REQUEST_UPDATE));
        }
        catch(Exception ex) {
            log.e(ex.getMessage());
        }
    }
    
    @Override
    public void run() {
        while(socket.isConnected()) {
            try {
                ServerCommand command = ServerCommand.receiveCommand(in);
                Object data = command.getData();
                
                log.i("Received: " + ServerCommand.getCommandDescription(command.getCommand()));
                
                switch(command.getCommand()) {
                    case ServerCommand.CMD_UPDATE:
                        fireOnUpdate((GameData)data);
                        break;
                    case ServerCommand.CMD_NOTIFY_TURN_BEGIN:
                        fireOnBeginTurn((Integer)data);
                        break;
                    case ServerCommand.CMD_BADREQUEST:
                        fireOnBadRequest((ServerCommand.BadRequestData)data);
                        break;
                    case ServerCommand.CMD_NOTIFY_MOVE_UNIT:
                        fireOnUnitMoved((ServerCommand.MoveUnitData)data);
                        break;
                    case ServerCommand.CMD_CLOSE:
                        socket.close();
                        break;
                    default:
                        throw new IOException("Invalid command: " + command.getCommand());
                }
            }
            catch(Exception ex) {
                log.e(ex.getMessage());
            }
        }
    }
    
    protected void fireOnBadRequest(ServerCommand.BadRequestData data) {
        for(Listener l : listeners)
            l.onBadRequest(this, data.RefCommand, data.ErrorCode);
    }
    
    protected void fireOnBeginTurn(int currentPlayer) {
        for(Listener l : listeners)
            l.onBeginTurn(this, currentPlayer);        
    }
    
    protected void fireOnUpdate(GameData gameData) {
        
        Player player = gameData.getPlayer(playerIndex);
        
        for(Listener l : listeners)
            l.onUpdate(this, player, gameData);
    }
    
    protected void fireOnUnitMoved(ServerCommand.MoveUnitData data) {
        for(Listener l : listeners)
            l.onUnitMoved(this, data.UnitID, data.FromPlanetID, data.ToPlanetID);
    }
    
    public static interface Listener {
        public void onUpdate(Client client, Player player, GameData gameData);
        public void onUnitMoved(Client client, int unitID, int fromPlanetID, int toPlanetID);
        public void onBeginTurn(Client client, int currentPlayer);
        public void onBadRequest(Client client, int refCommand, int errorCode);
    }
    
}
