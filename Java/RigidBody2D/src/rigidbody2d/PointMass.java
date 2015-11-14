/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package rigidbody2d;

import rigidbody2d.vecmath.Vector2D;

/**
 *
 * @author Andrea
 */
public class PointMass {
    
    private Vector2D position;
    private float mass;
    
    public PointMass(PointMass other) {
        this(new Vector2D(other.getPosition()), other.getMass());
        
    }
    
    public PointMass(Vector2D position, float mass) {
        this.position = position;
        this.mass = mass;
    }
    
    public PointMass(float x, float y, float mass) {
        this(new Vector2D(x, y), mass);
    }
    
    public Vector2D getPosition() { return position; }
    public float getMass() { return mass; }
}
