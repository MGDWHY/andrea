/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.server;

import java.io.File;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import projectcolossus.gamelogic.GameMap;

/**
 *
 * @author Andrea
 */
public class MatchmakingServer extends Thread {
    
    protected ArrayList<ClientHandler> clients;
    
    protected ServerSocket serverSocket;
    
    public MatchmakingServer(int port) throws IOException {
        serverSocket = new ServerSocket(port);
        clients = new ArrayList<ClientHandler>();
        start();
    }
    
    public void run() {
        while(isAlive()) {
            try {
                Socket socket = serverSocket.accept();
                clients.add(new ClientHandler(socket));
                
                if(clients.size() == 2) {
                    GameMap gameMap = GameMap.loadFromFile(new File("data/map.pcm"));
                    Server server = new Server(gameMap);
                    
                    for(ClientHandler c : clients) {
                        System.out.println("Starting game...");
                        ServerCommand.MMGameServerData data = new ServerCommand.MMGameServerData(server.getAddress(), server.getPort(), gameMap);
                        ServerCommand.sendCommand(ServerCommand.MMCMD_GAMESERVER, data, c.out);
                        c.socket.close();
                    }
                    
                    clients.clear();                
                }
            }
            catch(Exception ex) {
                ex.printStackTrace();
            }
        }
        
        try { serverSocket.close();}
        catch(IOException ex) {}
    }
    
    private class ClientHandler {
        
        public final Socket socket;
        
        public final ObjectOutputStream out;
        public final ObjectInputStream in;
        
        public ClientHandler(Socket socket) throws IOException {
            this.socket = socket;
            
            out = new ObjectOutputStream(socket.getOutputStream());
            in = new ObjectInputStream(socket.getInputStream());            
        }
    }
}
