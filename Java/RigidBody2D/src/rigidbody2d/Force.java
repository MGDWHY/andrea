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
public abstract class Force {
    /*
     * @return the values of a the force applied to the RigidBody b
     * at the MassPoint p
     */
    public abstract Vector2D getForce(RigidBody b, PointMass point);
}
