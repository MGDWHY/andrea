/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.map;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Location implements Serializable {
    
    private static final long serialVersionUID = 6733338686533680488L;
    
    private float x, y;
    
    public Location(float x, float y) {
        this.x = x;
        this.y = y;
    }
    
    public void setPosition(float x, float y) {
        this.x = x;
        this.y = y;
    }
    
    public float getX() { return x; }
    public float getY() { return y; }
    
    
}
