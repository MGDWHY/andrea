/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.util;

/**
 *
 * @author Andrea
 */
public class UIDGenerator {
    
    private int currentID = 1;
    
    public int newID() {
        return this.currentID++;
    }
}
