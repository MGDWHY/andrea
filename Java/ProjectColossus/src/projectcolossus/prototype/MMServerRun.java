/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.prototype;

import java.io.File;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.server.MatchmakingServer;
import projectcolossus.gamelogic.server.Server;

/**
 *
 * @author Andrea
 */
public class MMServerRun {
    public static final int SERVER_PORT = 10000;
    
    public static void main(String[] args) throws Exception {
        MatchmakingServer mmServer = new MatchmakingServer(SERVER_PORT);
        mmServer.join();
    }    
}
