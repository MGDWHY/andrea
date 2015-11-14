/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public class Player {
    private float radius;
    private Point2f position;
    
    public Player() {
        this.position = new Point2f();
    }
    
    public void setPostion(Point2f position) {
        this.position.x = position.x;
        this.position.y = position.y;
    }
    
    public void setPosition(float x, float y) {
        this.position.x = x;
        this.position.y = y;
    }
    
    public Point2f getPosition() { return position; }
    
    public void setRadius(float radius) { this.radius = radius; }
    
    public float getRadius() { return radius; }
    
    
}
