package rigidbody2d.vecmath;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Andrea
 */
public class Vector2D {
    
    public static final Vector2D ZERO = new Vector2D();
    
    public double x, y;
    
    public double getX() { return x; }
    public double getY() { return y; }
    
    public Vector2D(Vector2D other) {
        this(other.x, other.y);
    }
    
    public Vector2D() { this(0, 0); }
    
    public Vector2D(double x, double y) {
        this.x = x;
        this.y = y;
    }
    
    public void set(double x, double y) {
        this.x = x;
        this.y = y;
    }
    
    public void set(Vector2D other) {
        set(other.x, other.y);
    }
    
    public Vector2D add(Vector2D other) { return new Vector2D(x + other.x, y + other.y); }
    public Vector2D sub(Vector2D other) { return new Vector2D(x - other.x, y - other.y); }
    
    public Vector2D scale(double factor) { return new Vector2D(x * factor, y * factor); }
    public Vector2D divide(double factor) { return scale(1 / factor); }
    
    public Vector2D perp() { return new Vector2D(-y, x); }
    
    public double dot(Vector2D other) { return x * other.x + y * other.y; }
    
    public Vector2D opposite() { return new Vector2D(-x, -y); }
    
    public Vector2D normalize() {
        double len = length();
        return new Vector2D(x / len, y / len);
    }
    
  
    
    public double length() {
        return Math.sqrt(x*x + y*y);
    }
    
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
