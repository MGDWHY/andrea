/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net.server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import projectconquest.data.map.Map;
import projectconquest.net.DataChannel;
import projectconquest.net.DataSocket;
import projectconquest.net.Protocol;
import projectconquest.net.ProtocolException;
import projectconquest.util.ConsoleLog;
import projectconquest.util.Log;
import projectconquest.util.UIDGenerator;

/**
 *
 * @author Andrea
 */
public class GameServer extends Thread {
    
    private Log log;
    
    private UIDGenerator uidgen;
    
    private int currentPlayerTurn;
    
    private int serverPort;
    
    private int playerTickets[];
    private boolean playerConnected[];
    private DataChannel[] playerDataChannels;
    private Player[] players;
    
    private GameThread[] gameThreads;
    
    private Map map;
    
    public GameServer(Map map, int serverPort) {
        
        log = new ConsoleLog();
        
        uidgen = new UIDGenerator();
        
        playerTickets = new int[map.getPlayerNumber()];
        playerConnected = new boolean[map.getPlayerNumber()];
        playerDataChannels = new DataChannel[map.getPlayerNumber()];
        players = new Player[map.getPlayerNumber()];
        
        gameThreads = new GameThread[map.getPlayerNumber()];
        
        this.serverPort = serverPort;
        this.map = map;
        
    }
    
    public void setPlayerTicket(int index, int ticket) { playerTickets[index] = ticket; }
    
    
    public void run() {
        try {
            Thread waitForPlayers = new WaitForPlayersThread();
            waitForPlayers.setName("ListeningThread");
            waitForPlayers.start();
            waitForPlayers.join();
        }
        catch(InterruptedException ex) {
            log.e(this, "Interruped.");
        }
    }
    
    protected int connect(Socket s) throws IOException {
        DataSocket clientChannel = null, serverChannel = null;
        int serverChannelPort = -1;        
        
        try {

            clientChannel = new DataSocket(s);
            clientChannel.assertGSCommand(Protocol.M_GS_CONNECT);
            clientChannel.writeAck();

            int ticket = clientChannel.readInt();
            int playerIndex = checkTicket(ticket);
            clientChannel.writeAck();

            serverChannelPort = clientChannel.readInt();
            serverChannel = new DataSocket(new Socket(s.getInetAddress(), serverChannelPort));
            clientChannel.writeAck();

            playerConnected[playerIndex] = true;
            players[playerIndex] = new Player(playerIndex);         
            playerDataChannels[playerIndex] =  new DataChannel(clientChannel, serverChannel);
            
            log.i(this, "Player connected.");
            log.i(this, "Ticket: " + ticket);
            log.i(this, "Index: " + playerIndex);
            
            return playerIndex;

        }
        catch(ProtocolException ex) {
            log.e(this, "Player not connected: " + ex.getMessage());
            clientChannel.writeNack(ex.getMessage());
            return -1;
        }
    
    }    
    
    protected int checkTicket(int ticket) {
        
        for(int i = 0; i < map.getPlayerNumber(); i++) {
            if(playerTickets[i] == ticket && !playerConnected[i])
                return i;
        }
        
        return -1;
    }
    
    protected boolean isGameReady() {
        boolean result = true;
        for(int i = 0; i < map.getPlayerNumber(); i++)
            result = result & playerConnected[i];
        
        return result;
    }
    
    protected void srvbc_waitForPlayers() throws IOException {
        for(int i = 0; i < map.getPlayerNumber(); i++) {
            if(playerConnected[i])
                srv_waitForPlayers(i);
        }
    }
    
    protected void srv_waitForPlayers(int playerIndex) throws IOException {
        DataSocket sc = playerDataChannels[playerIndex].getServerChannel();
        
        sc.writeGSCommand(Protocol.M_GS_WAIT_FOR_PLAYERS);
        try {
            sc.readResponse().assertOk();
        }
        catch(ProtocolException ex) {
            log.e(this, ex.getMessage());
        }
    }
    
    protected void srvbc_runGame() throws IOException {
        for(int i = 0; i < map.getPlayerNumber(); i++) {
            if(playerConnected[i])
                srv_runGame(i);
        }        
    }
    
    protected void srv_runGame(int playerIndex) throws IOException {
        DataSocket sc = playerDataChannels[playerIndex].getServerChannel();
        
        sc.writeGSCommand(Protocol.M_GS_RUN_GAME);
        try {
            sc.readResponse().assertOk();
        }
        catch(ProtocolException ex) {
            log.e(this, ex.getMessage());
        }        
    }
   
    
    protected void srvbc_endTurn() throws IOException {
        for(int i = 0; i < map.getPlayerNumber(); i++) {
            try {
                playerDataChannels[i].getServerChannel().writeGSCommand(Protocol.M_GS_END_TURN);
                playerDataChannels[i].getServerChannel().readResponse().assertOk();
            }
            catch(ProtocolException ex) {
                log.e(this, ex.getMessage());
            }
        }
    }
    
    protected void handlePlayerCommand(int playerIndex) throws IOException {
     
        DataSocket cc = playerDataChannels[playerIndex].getClientChannel();
        int cmd;
        
        try {cmd = cc.readGSCommand();}
        catch(ProtocolException ex) {
            cc.writeNack(ex.getMessage());
            return;
        }
        
        switch(cmd) {
            case Protocol.M_GS_GET_MAP:
                cc.writeAck();
                cc.writeUTF(map.getName());
                break;      
                
            case Protocol.M_GS_IS_MY_TURN:
                cc.writeAck();
                cc.writeInt(playerIndex == currentPlayerTurn ? 1 : 0);
                break;
                    
            case Protocol.M_GS_END_TURN:
                if(playerIndex == currentPlayerTurn) {
                    cc.writeAck();
                    currentPlayerTurn = (currentPlayerTurn + 1) % map.getPlayerNumber();
                    srvbc_endTurn();
                } else
                    cc.writeNack("It's not your turn");
                break;

            default:
                cc.writeNack("Invalid command: " + cmd);
                return;
        }
    }
    
    protected boolean checkTurn(int playerIndex) {
        if(playerIndex != currentPlayerTurn)
            return false;
        else
            return true;
    }
    
    protected class WaitForPlayersThread extends Thread {
        
        public void run() {
            ServerSocket ss = null;
            try { ss = new ServerSocket(serverPort); }
            catch(IOException ex) {ex.printStackTrace(); }
            
            while(true) {
                try { 
                    int playerIndex = connect(ss.accept());
                    if(playerIndex != -1) {
                        gameThreads[playerIndex] = new GameThread(playerIndex);
                        gameThreads[playerIndex].setName("Player" + playerIndex + "Thread");
                        gameThreads[playerIndex].start(); 
                    }
                    
                    if(isGameReady())
                        srvbc_runGame();
                    else
                        srvbc_waitForPlayers();
                    
                }
                catch(IOException ex) {
                    ex.printStackTrace();
                }                
            }
        }
    }

    protected class GameThread extends Thread {
        
        private Object lock;
        
        private int playerIndex;
        
        public GameThread(int playerIndex) {
            this.playerIndex = playerIndex;
            this.lock = new Object();
        }
        
        public void run() {
            
            log.i(this, "Player " + playerIndex + " thread is running");
            
            while(true) {

                try {              
                    handlePlayerCommand(playerIndex);
                }
                catch(IOException ex) {
                    log.e(this, ex.getMessage());
                    playerConnected[playerIndex] = false;
                    playerDataChannels[playerIndex] = null;
                    gameThreads[playerIndex] = null;
                    try {
                        srvbc_waitForPlayers();
                    }
                    catch(IOException ex2) {
                        log.e(this, ex2.getMessage());
                    }
                    return;
                }
            }
        }
    }

    
}
