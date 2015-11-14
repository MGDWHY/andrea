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
public class ConstantAppliedForce extends AppliedForce {
    
    protected Vector2D force;
    
    public ConstantAppliedForce(PointMass point, Vector2D force) {
        super(point);
        this.force = force;
    }

    @Override
    public Vector2D getForce(RigidBody b) {
        return force;
    }
    
}
