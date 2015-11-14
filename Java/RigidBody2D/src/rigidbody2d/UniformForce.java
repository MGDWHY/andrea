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
public class UniformForce extends Force {
    
    public static final UniformForce GRAVITY = new UniformForce(new Vector2D(0.0f, -9.8f));
    
    private Vector2D force;
    
    public UniformForce(Vector2D force) {
        this.force = force;
    }

    @Override
    public Vector2D getForce(RigidBody b, PointMass point) {
        return force;
    }

}