/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.server;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import projectcolossus.gamelogic.GameMap;

/**
 *
 * @author Andrea
 */
public class ServerCommand implements Serializable {
    
    /* Matchmaking Server commands*/
    
    /*
     * Command: Game Server
     * Desc: sent from the mm server to the client to tell him where's the game server
     * Data: Address, Port, GameMap encolosed in a MMGameServerData instance
     */
    public static final int MMCMD_GAMESERVER = 100;
    
    /* Game Server commands */
    
    
    /**
     * Command: close
     * Desc: sent by the client to notify the connection end.
     * Data: NULL
     */
    public static final int CMD_CLOSE = 301;
    
    /**
     * Command: end turn
     * Desc: Sent by the client to the server to notify the end of the
     * current player's turn
     * Data: NULL
     */
    public static final int CMD_END_TURN = 200;
    
    /**
     * Command: update
     * Bidirectional command used to send a full game-state update
     * Data: instance of GameData
     */
    public static final int CMD_UPDATE = 201;  
    
    /**
     * Command: request update
     * Desc: Sent by the client to the server to ask for a full game-state update
     * Data: NULL
     */
    public static final int CMD_REQUEST_UPDATE = 202;
    
    /**
     * Command: notify begin turn
     * Desc: sent by the server to notify a client that his turn has begun
     * Data: Integer, index of the player that is beginning the turn
     */
    public static final int CMD_NOTIFY_TURN_BEGIN = 203;
    
    /**
     * Command: move unit
     * Desc: sent by the client to server to move a player's unit
     * Data: UnitID, FromPlanetID, ToPlanetID enclosed in a MoveUnitData instance
     */
    public static final int CMD_MOVE_UNIT = 204;
    
    /**
     * Command: play free card
     * Desc: sent by the client to play a free card (instance of FreeCard)
     * Data: Integer, index of the card to play in the player's hand
     */
    public static final int CMD_PLAY_FREE_CARD = 205;
    
    /**
     * Command: play planet card
     * Desc: sent by the client to play a card on a planet (instance of PlanetCard)
     * Data: cardIndex (integer), PlanetID (integer) enclosed in a PlayPlanetCardData instance
     */
    public static final int CMD_PLAY_PLANET_CARD = 206;
    
    /**
     * Command: notify move unit
     * Desc: sent by the server to the clients to notify that a unit has been moved (it is sent
     * to the unit's owner too
     * Data: same as CMD_MOVE_UNIT
     * 
     */
    public static final int CMD_NOTIFY_MOVE_UNIT = 207;
    
    /**
     * Command: bad request
     * Desc: sent by the server to client when some operation can't be executed
     * Data: RefCommand ( the command that caused the exception), ErrorCode
     * enclosed in a BadRequestData instance
     */
    public static final int CMD_BADREQUEST = 400;
    
    // Error codes
    public static final int ERR_INVALID_ACTION = 1;
    public static final int ERR_INVALID_COMMAND = 2;
    
    protected int command;
    protected Object data;
    
    public ServerCommand(int command, Object data) {
        this.command = command;
        this.data = data;
    }
    
    public int getCommand() { return command; }
    public Object getData() { return data; }
    
    public static void sendCommand(ServerCommand command, ObjectOutputStream out, boolean threaded) throws IOException {
        if(threaded) {
            new SendThreadedCommand(command, out).start(); 
        } else {
            out.reset();
            out.writeObject(command);
            out.flush();
        }
    }    
    
    public static void sendCommand(int command, Object data, ObjectOutputStream out, boolean threaded) throws IOException {
        sendCommand(new ServerCommand(command, data), out, threaded);
    }
    
    public static void sendCommand(int command, Object data, ObjectOutputStream out) throws IOException {
        sendCommand(command, data, out, false);
    }
    
    public static ServerCommand receiveCommand(ObjectInputStream in) throws IOException, ClassNotFoundException {
        return (ServerCommand) in.readObject();
    }
    
    public static String getCommandDescription(int command) {
        switch(command) {
            case MMCMD_GAMESERVER: return "GAME_SERVER";
            case CMD_CLOSE: return "CLOSE";
            case CMD_BADREQUEST: return "BAD_REQUEST";
            case CMD_END_TURN: return "END_TURN";
            case CMD_MOVE_UNIT: return "MOVE_UNIT";
            case CMD_NOTIFY_TURN_BEGIN: return "NOTIFY_TURN_BEGIN";
            case CMD_PLAY_FREE_CARD: return "PLAY_FREE_CARD";
            case CMD_PLAY_PLANET_CARD: return "PLAY_PLANET_CARD";
            case CMD_REQUEST_UPDATE: return "REQUEST_UPDATE";
            case CMD_UPDATE: return "UPDATE";
            default: return "UNKNOWN COMMAND (" + command + ")";
        }
    }
    
    public static class MMGameServerData implements Serializable {
        public String ServerAddress;
        public int ServerPort;
        public GameMap GameMap;
        
        public MMGameServerData(String serverAddres, int serverPort, GameMap gameMap) {
            this.ServerAddress = serverAddres;
            this.ServerPort = serverPort;
            this.GameMap = gameMap;
        }
    }
    
    public static class PlayPlanetCardData implements Serializable {
        public int CardIndex;
        public int PlanetID;
        
        public PlayPlanetCardData(int cardIndex, int planetID) {
            this.CardIndex = cardIndex;
            this.PlanetID = planetID;
        }
    }
    
    public static class MoveUnitData implements Serializable {
        public int UnitID;
        public int FromPlanetID;
        public int ToPlanetID;
        
        public MoveUnitData(int unitID, int fromPlanetID, int toPlanetID) {
            this.UnitID = unitID;
            this.FromPlanetID = fromPlanetID;
            this.ToPlanetID = toPlanetID;
        }
    }
    
    public static class BadRequestData implements Serializable {
        public int RefCommand;
        public int ErrorCode;
        
        public BadRequestData(int refCommand, int errorCode) {
            this.RefCommand = refCommand;
            this.ErrorCode = errorCode;
        }
    }
    
    private static class SendThreadedCommand extends Thread {
        
        private ObjectOutputStream out;
        private ServerCommand command;
        
        public SendThreadedCommand(ServerCommand command, ObjectOutputStream out) {
            this.command = command;
            this.out = out;
        }
        
        public void run() {
            try {
                sendCommand(command, out, false);
            }
            catch(IOException ex) {
                ex.printStackTrace();
            }
        }
    }
    
}
