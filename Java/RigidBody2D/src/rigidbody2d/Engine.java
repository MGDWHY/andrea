/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package rigidbody2d;

import java.util.List;
import java.util.ArrayList;
import rigidbody2d.vecmath.Vector2D;

/**
 *
 * @author Andrea
 */
public class Engine {
    
    private Object lock;
   
    private boolean isRunning;
    
    private ArrayList<RigidBody> rigidBodies;
    
    private EngineThread engineThread;
    
    public Engine() {
        lock = new Object();
        isRunning = false;
        rigidBodies = new ArrayList<>();
        engineThread = new EngineThread();
        
        engineThread.start();
        
    }
    
    public List<RigidBody> getRigidBodies() { return rigidBodies; }
    
    public void start() {
        synchronized(lock) {
            isRunning = true;
            notifyAllOnLock();
        }
    }
    
    public void pause() {
        synchronized(lock) {
            isRunning = false;
            notifyAllOnLock();
        }
    }
    
    public boolean addRigidBody(RigidBody b) { return rigidBodies.add(b); }
    public boolean removeRigidBody(RigidBody b) { return rigidBodies.remove(b); }
    
    private class EngineThread extends Thread {
        public void run() {
            
            long prevTime = System.currentTimeMillis();
            
            while(true) {   
                synchronized(lock) {
                    while(!isRunning) {
                        waitOnLock();
                        prevTime = System.currentTimeMillis();
                    }
                }
                
                float dt = (System.currentTimeMillis() - prevTime) / 1000.0f;
                prevTime = System.currentTimeMillis();
                
                /* Resolve collisions */
                
                for(int i = 0; i < rigidBodies.size(); i++)
                    for(int j = 0; j < rigidBodies.size(); j++)
                        if(i != j) {
                            RigidBody.CollisionInfo info = rigidBodies.get(i).checkCollision(rigidBodies.get(j));
                            if(info.colliding) {
                                RigidBody b0 = rigidBodies.get(i);
                                RigidBody b1 = rigidBodies.get(j);
                                //Vector2D v0 = b0.getPointVelocity(info.collisionPoint);
                                //Vector2D v1 = b1.getPointVelocity(info.collisionPoint);
                                Vector2D v0 = b0.getVelocity();
                                Vector2D v1 = b1.getVelocity();                           
                                Vector2D v01 = v0.sub(v1);
                                if(v01.dot(info.collisionAxis) > 0) {
                                    Vector2D r0 = info.collisionPoint.sub(b0.getCenterOfMass());
                                    Vector2D r1 = info.collisionPoint.sub(b1.getCenterOfMass());
                                    
                                    double impulse = -2 * v01.dot(info.collisionAxis) / (
                                                1/b0.getTotalMass() + 1/b0.getTotalMass() +
                                                Math.pow(r0.dot(info.collisionAxis), 2) / b0.getInertia() +
                                                Math.pow(r1.dot(info.collisionAxis), 2) / b1.getInertia()
                                            );
                                    
                                    b0.setVelocity(v0.add(info.collisionAxis.scale(impulse / b0.getTotalMass())));
                                    b1.setVelocity(v1.add(info.collisionAxis.scale(-impulse / b1.getTotalMass())));
                                    
                                    b0.setAngularVelocity(b0.getAngularVelocity() + r0.perp().dot(info.collisionAxis.scale(impulse)) / b0.getInertia());
                                    b1.setAngularVelocity(b1.getAngularVelocity() + r1.perp().dot(info.collisionAxis.scale(-impulse)) / b1.getInertia());

                                }
                            }
                            
                        }
                
                /* Update rigid bodies */
                for(RigidBody b : rigidBodies)
                    b.update(dt);
                
                
            }
        }
    }
    
    private void notifyAllOnLock() {
        lock.notifyAll();
    }
        
    private void waitOnLock() {
        try { lock.wait(); }
        catch(InterruptedException ex) { ex.printStackTrace(); }
    }
}
