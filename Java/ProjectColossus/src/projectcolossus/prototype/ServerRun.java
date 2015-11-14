/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.prototype;

import java.io.File;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.server.Server;

/**
 *
 * @author Andrea
 */
public class ServerRun {
    
    public static final int SERVER_PORT = 10000;
    
    public static void main(String[] args) throws Exception {
        GameMap gameMap = GameMap.loadFromFile(new File(GameProto1.MAP_FILE));
        Server server = new Server(SERVER_PORT, gameMap);
        server.join();
    }
}
