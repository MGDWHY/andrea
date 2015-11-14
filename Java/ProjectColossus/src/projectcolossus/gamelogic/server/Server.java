/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.server;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.cards.FreeCard;
import projectcolossus.gamelogic.player.HumanPlayer;
import projectcolossus.gamelogic.units.Unit;

/**
 *
 * @author Andrea
 */
public class Server extends Thread {  
    protected int connectedPlayers;
    
    protected GameData gameData;
    
    protected GameMap gameMap;
    
    protected ServerSocket serverSocket;
    
    protected int port;
 
    protected ArrayList<ClientHandler> clients; // TODO sostituire ArrayList con semplice array associativo [index -> player]. Possibile gestione della connessione con tickets
    
    protected Object lock; // Servirà in seguito per sincronizzare
    
    public Server(GameMap gameMap) throws IOException {
        this(0, gameMap);
    }
    
    public Server(int port, GameMap gameMap) throws IOException {
        this.port = port;
        this.serverSocket = new ServerSocket(port);
        this.gameMap = gameMap;
        this.clients = new ArrayList<ClientHandler>();
        this.connectedPlayers = 0;
        this.lock = new Object();
        start();
    }
    /*
     * TODO Verificare se è un doppioni di getListeningPort()
     */
    public int getPort() {
        return serverSocket.getLocalPort();
    }
    
    /**
     * Returns the IP address of this server
     * @return 
     */
    public String getAddress() {
        return serverSocket.getInetAddress().getHostName();
    }
    
    /**
     * Returns the server listening port
     * @return the server port
     */
    public int getListeningPort() {
        return port;
    }

    /* Da rifare completamente. Deve cercare un indice libero e assegnarlo (facile appena si sostituisce la ArrayList con array */
    public int nextIndex() {
        return connectedPlayers++;
    }
    
    /**
     * Wait for players, create clients' handlers and start the game 
     */
    @Override
    public void run() {
        while(connectedPlayers < gameMap.getPlayerNumber()) {
            try {
                Socket socket = serverSocket.accept();
                clients.add(new ClientHandler(socket));
            }
            catch(IOException ex) {
                ex.printStackTrace();
            }
        }
        
        gameData = new GameData(gameMap);
        
        for(int i = 0; i < clients.size(); i++)
            gameData.setPlayer(clients.get(i).getPlayer());
        
        
        gameData.beginTurn();
        
        for(Player player: gameData.getPlayers())
            player.play();
        
        sendBroadcastUpdate();
        notifyBeginTurn();
        
        for(ClientHandler c : clients)
            c.start();
        
        try {
            for(ClientHandler c: clients)
                c.join();
        }
        catch(InterruptedException ex) {
            ex.printStackTrace();
        }
        
        try { serverSocket.close(); }
        catch(IOException ex) {}
    }
    
    /**
     * Gets the client handler associated with the given player
     * @param player The player
     * @return a ClientHandler
     */
    protected ClientHandler getClient(Player player) {
        for(ClientHandler c : clients)
            if(c.player.equals(player))
                return c;
        
        return null;
    }
    /*
     * Helper functions to send commands to the clients
     * TODO Aggiungere throws IOException a tutti i commands (Oppure log)
     */
    protected void notifyBeginTurn() {
        try {
            ClientHandler cp = getClient(gameData.getCurrentPlayer());                
            ServerCommand.sendCommand(ServerCommand.CMD_NOTIFY_TURN_BEGIN, new Integer(gameData.getCurrentPlayer().getIndex()), cp.out);            
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }
    protected void sendBroadcast(int command, Object data) {
        try {
            for(ClientHandler c : clients)
                ServerCommand.sendCommand(new ServerCommand(command, data), c.out, false);
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }        
    }    
    protected void sendBroadcast(ServerCommand command) {
        try {
            for(ClientHandler c : clients)
                ServerCommand.sendCommand(command, c.out, false);
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }        
    } 
    protected void sendBroadcastUpdate() {
        sendBroadcast(ServerCommand.CMD_UPDATE, gameData);
    }  
    protected void sendBadRequest(int refCommand, int errorCode, ClientHandler client) throws IOException {
        ServerCommand.BadRequestData brData = new ServerCommand.BadRequestData(refCommand, errorCode);
        ServerCommand.sendCommand(ServerCommand.CMD_BADREQUEST, brData, client.out);
    }       
    
    /*
     * Class that handles a client
     */
    private class ClientHandler extends Thread {
        
        protected Socket socket;
        
        protected Player player;
        
        protected ObjectInputStream in;
        protected ObjectOutputStream out;
        
        // TOTO creare dei comandi appositi per iniziare una connessione
        public ClientHandler(Socket socket) throws IOException {
            
            this.socket = socket;

            out = new ObjectOutputStream(socket.getOutputStream());            
            in = new ObjectInputStream(socket.getInputStream());

            out.flush();
            
            int index = nextIndex();
            
            String name = in.readUTF();
            Deck deck = Deck.read(in);
            
            deck.shuffle();          
            
            out.writeInt(index);
            out.flush();
            
            player = new HumanPlayer(index, name);
            player.setDeck(deck);
        }
        

        public Player getPlayer() { return player; }
        /*
         * Server commands handlers
         */
        protected void handlePlayPlanetCard(ServerCommand.PlayPlanetCardData data) throws IOException {
            Card card = gameData.getPlayer(player.getIndex()).getHand().get(data.CardIndex);
            Planet planet = gameData.getPlanetByID(data.PlanetID);
            
            if(card == null || planet == null) {
                sendBadRequest(ServerCommand.CMD_PLAY_PLANET_CARD, ServerCommand.ERR_INVALID_COMMAND, this);
                return;
            }
            
            if(card.isPlayable() && card.canPlayOnPlanet(gameMap, planet)) {
                card.playOnPlanet(gameMap, planet);
                sendBroadcastUpdate();
            } else {
                sendBadRequest(ServerCommand.CMD_PLAY_PLANET_CARD, ServerCommand.ERR_INVALID_ACTION, this);
            }
            
        }
        protected void handleMoveUnit(ServerCommand.MoveUnitData data) throws IOException {
            Unit unit = gameData.getCurrentPlayer().getUnitByID(data.UnitID);
            Planet from = gameData.getPlanetByID(data.FromPlanetID);
            Planet to = gameData.getPlanetByID(data.ToPlanetID);

            if(unit != null && from != null && to != null) {
                boolean result = unit.moveTo(gameData.getGameMap(), from, to);

                if(result) {
                    sendBroadcast(ServerCommand.CMD_NOTIFY_MOVE_UNIT, data);
                    sendBroadcastUpdate();
                } else {
                    sendBadRequest(ServerCommand.CMD_MOVE_UNIT, ServerCommand.ERR_INVALID_ACTION, this);
                }
            } else {
                sendBadRequest(ServerCommand.CMD_MOVE_UNIT, ServerCommand.ERR_INVALID_COMMAND, this);
            }            
        }
        
        /*
         * Server's main loop
         */
        @Override
        public void run() { // TODO occhio -> da sincronizzare
            try { // TODO non mi piace questo try enorme che racchiude il ciclo while! da riverede
                while(socket.isConnected()) {
                    
                    ServerCommand command = ServerCommand.receiveCommand(in);
                    Object data = command.getData();

                    switch(command.getCommand()) {
                        case ServerCommand.CMD_END_TURN:     
                            
                            if(!gameData.getCurrentPlayer().equals(player))
                                continue;
                            
                            gameData.playTurn();
                            gameData.endTurn();
                            gameData.beginTurn();
                            gameData.getCurrentPlayer().play();
                            
                            sendBroadcastUpdate();
                            notifyBeginTurn();
                                                       
                            break;
                        case ServerCommand.CMD_UPDATE:
                            if(!gameData.getCurrentPlayer().equals(player))
                                continue;   
                            
                            gameData = (GameData) data;
                            sendBroadcastUpdate();
                            break;
                        case ServerCommand.CMD_MOVE_UNIT:
                            if(!gameData.getCurrentPlayer().equals(player))
                                continue;   
                            
                            handleMoveUnit((ServerCommand.MoveUnitData) data);
                            
                            break;
                        case ServerCommand.CMD_PLAY_FREE_CARD:
                            if(!gameData.getCurrentPlayer().equals(player))
                                continue;                               
                            
                            int index = (Integer) data;
                            
                            Card card = gameData.getPlayer(player.getIndex()).getHand().get(index);
                            
                            if(card != null && card instanceof FreeCard && card.isPlayable()) {
                                FreeCard freeCard = (FreeCard) card;
                                card.play(gameData.getGameMap());
                                sendBroadcastUpdate();
                            } else {           
                                sendBadRequest(ServerCommand.CMD_PLAY_FREE_CARD, ServerCommand.ERR_INVALID_COMMAND, this);
                            }          
                            break;
                        case ServerCommand.CMD_PLAY_PLANET_CARD:
                            if(!gameData.getCurrentPlayer().equals(player))
                                continue;   
                            
                            handlePlayPlanetCard((ServerCommand.PlayPlanetCardData)data);
                            
                        case ServerCommand.CMD_REQUEST_UPDATE:
                            ServerCommand.sendCommand(ServerCommand.CMD_UPDATE, gameData, out);
                            break;
                        case ServerCommand.CMD_CLOSE:
                            ServerCommand.sendCommand(ServerCommand.CMD_CLOSE, null, out);
                            socket.close();
                        default:
                            //socket.close();
                            break;
                    }
                }
            }
            catch(Exception ex) {
                ex.printStackTrace();
            }
        }
    }
}
