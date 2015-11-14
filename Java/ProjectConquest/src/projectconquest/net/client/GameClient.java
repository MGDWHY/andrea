/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net.client;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import projectconquest.net.DataChannel;
import projectconquest.net.DataSocket;
import projectconquest.net.Protocol;
import projectconquest.net.ProtocolException;
import projectconquest.util.ConsoleLog;
import projectconquest.util.Log;

/**
 *
 * @author Andrea
 */
public class GameClient extends Thread {
    
    public static final int LISTENING_PORT = 10000;
    
    protected int ticket;
    
    protected DataChannel dataChannel;
    
    protected Log log;
    
    protected ArrayList<Listener> listeners;
    
    public GameClient() {
        log = new ConsoleLog();
        listeners = new ArrayList<>();
    }
    
    public boolean addListener(Listener l) { return listeners.add(l); }
    public boolean removeListener(Listener l) { return listeners.remove(l); }
    
    public void setTicket(int ticket) { this.ticket = ticket; }
    
    public void connect(InetAddress serverAddress, int serverPort) throws IOException {
        DataSocket clientChannel = new DataSocket(new Socket(serverAddress, serverPort));
        ServerSocket ss = new ServerSocket(LISTENING_PORT);
        
        try {
            
            log.i(this, "Initializing connection protocol");
            
            clientChannel.writeGSCommand(Protocol.M_GS_CONNECT);
            clientChannel.readResponse().assertOk();
            
            log.i(this, "Ack #1 received");
 
            clientChannel.writeInt(ticket);
            clientChannel.readResponse().assertOk();
            
            log.i(this, "Ack #2 received");
            
            clientChannel.writeInt(LISTENING_PORT);
            DataSocket serverChannel = new DataSocket(ss.accept()); 
            clientChannel.readResponse().assertOk();
            
            log.i(this, "Ack #3 received");
            log.i(this, "Connected");
            
            ss.close();
            
            dataChannel = new DataChannel(clientChannel, serverChannel);
 
            start();
            
        }
        catch(ProtocolException ex) {
            log.e(this, ex.getMessage());
        }
    }
    
    public synchronized void endTurn() throws IOException {
        DataSocket cc = dataChannel.getClientChannel();
        cc.writeGSCommand(Protocol.M_GS_END_TURN);
        
        try {
            cc.readResponse().assertOk();
        }
        catch(ProtocolException ex) {
            log.e(this, "It's not your turn");
        }
    }
    
    public synchronized String getMapName() throws IOException {
        DataSocket cc = dataChannel.getClientChannel();
        cc.writeGSCommand(Protocol.M_GS_GET_MAP);
        try {
            cc.readResponse().assertOk();
            String name = cc.readUTF();
            return name;
        }
        catch(ProtocolException ex) {
            log.e(this, ex.getMessage());
            return null;
        }
    }
    
    public synchronized boolean isMyTurn() throws IOException {
        DataSocket cc = dataChannel.getClientChannel();
        
        cc.writeGSCommand(Protocol.M_GS_IS_MY_TURN);
        
        try {
            cc.readResponse().assertOk();
            if(cc.readInt() == 1)
                return true;
            else
                return false;
        }
        catch(ProtocolException ex) {
            log.e(this, "GameClient.isMyTurn(): " + ex.getMessage());
            return false;            
        }
    }
    
    public void run() {
        while(true) {
            try { processServerCommand(); }
            catch(IOException ex) {
                log.e(this, ex.getMessage());
            }
        }
    }
    
    protected void processServerCommand() throws IOException {
             
        DataSocket sc = dataChannel.getServerChannel();
        try {
            int cmd = sc.readGSCommand();
            GameClientCommand cmdObj = new GameClientCommand(cmd);
            
            switch(cmd) {
                case Protocol.M_GS_END_TURN:
                    sc.writeAck();
                    fireCommandReceived(cmdObj);
                    break;
                    
                case Protocol.M_GS_WAIT_FOR_PLAYERS:
                    sc.writeAck();
                    fireCommandReceived(cmdObj);
                    break;
                    
                case Protocol.M_GS_RUN_GAME:
                    sc.writeAck();
                    fireCommandReceived(cmdObj);
                    break;                    
                    
                default:                    
                    sc.writeNack("Invalid command: " + cmd);
            }
            
        }
        catch(ProtocolException ex) {
            log.e(this, ex.getMessage());
        }
    }
    
    protected void fireCommandReceived(final GameClientCommand cmd) {
        new Thread() {
            public void run() {
                for(Listener l : listeners)
                    l.commandRecevied(cmd);                
            }
        }.start();
    }
    
    public static interface Listener {
        public void commandRecevied(GameClientCommand cmd);
    }
    
}
