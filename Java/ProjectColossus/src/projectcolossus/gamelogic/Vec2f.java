/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Vec2f implements Serializable {
    private static final long serialVersionUID = -3799506220770781661L;

    
    private float x, y;
    
    public Vec2f() {
        this(0, 0);
    }
    
    public Vec2f(float x, float y) {
        this.x = x;
        this.y = y;
    }
    
    public Vec2f substract(Vec2f other) {
        return new Vec2f(x - other.x, y - other.y);
    }
    
    public float length() {
        return (float)Math.sqrt(x*x + y*y);
    }
    
    public float getX() {
        return x;
    }
    
    public float getY() {
        return y;
    }
    
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
