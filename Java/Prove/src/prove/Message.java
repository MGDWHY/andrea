/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package prove;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Message implements Serializable {
    
    private int id;
    private String message;
    
    public Message(int id, String message) {
        this.id = id;
        this.message = message;
    }
    
    @Override
    public String toString() {
        return id + ": " + message;
    }
    
}
