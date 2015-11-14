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
public abstract class AppliedForce extends Force {
    
    protected PointMass pointMass;
    
    public AppliedForce(PointMass p) {
        this.pointMass = p;
    }

    @Override
    public Vector2D getForce(RigidBody b, PointMass point) {
        if(point == this.pointMass)
            return getForce(b);
        else
            return Vector2D.ZERO;
    }
    
    
    public abstract Vector2D getForce(RigidBody b);
    
}
