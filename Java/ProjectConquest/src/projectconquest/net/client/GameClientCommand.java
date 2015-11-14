/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net.client;

import java.util.HashMap;

/**
 *
 * @author Andrea
 */
public class GameClientCommand {
    
    /* Comamnd parameters */
    public static String P_PLAYER_INDEX = "playerIndex";
    
    protected int command;
    protected HashMap<String, Object> parameters;
    
    public GameClientCommand(int command) {
        this.command = command;
        this.parameters = new HashMap<>();
    }
    
    public int getCommand() { return command; }
    
    public void putParameter(String key, Object value) {  parameters.put(key, value); }
    public Object getParameter(String key) { return parameters.get(key); }
    
}
