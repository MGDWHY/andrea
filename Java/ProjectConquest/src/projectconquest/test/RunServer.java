/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.test;

import java.io.FileInputStream;
import java.io.ObjectInputStream;
import projectconquest.data.map.Map;
import projectconquest.net.server.GameServer;

/**
 *
 * @author Andrea
 */
public class RunServer {
    
    public static final int SERVER_PORT = 12000;
    
    public static void main(String[] args) throws Exception {
        
        ObjectInputStream is = new ObjectInputStream(new FileInputStream("provamappa.pcm"));
        Map map = (Map) is.readObject();
        is.close();
        
        GameServer s = new GameServer(map, SERVER_PORT);
        s.start();
        s.join();
    }
}
