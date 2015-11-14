/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package rigidbody2d;

import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.geom.AffineTransform;
import java.awt.geom.PathIterator;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import rigidbody2d.vecmath.Vector2D;

/**
 *
 * @author Andrea
 */
public class RigidBody implements Shape {
    
    // Velocity and angular velocity;
    private Vector2D v;
    private double w;
    
    private float totMass; // total mass
    private Vector2D cm; // center of mass;
    private float icm; // moment of inertia of the center of mass
    
    private PointMass[] pointsMass;
    
    private ArrayList<Force> forces;
    
    private PathIteratorImpl defPathIt;
    
    public RigidBody(PointMass[] points) {
        this(new Vector2D(), 0, points);
    }
    
    public RigidBody(Vector2D v0, float w0, PointMass[] points) {
        this.defPathIt = new PathIteratorImpl();
        this.pointsMass = points;
        this.forces = new ArrayList<>();
        this.v = v0;
        this.w = w0;
        
        initialize();        
    }

    
    public void update(float dt) {
        /* Apply forces */
        Vector2D totalForce = new Vector2D();
        float totalTorque = 0.0f;
        
        for(Force f: forces) 
            for(PointMass p : pointsMass) {
                totalForce = totalForce.add(f.getForce(this, p));
                totalTorque += cm.sub(p.getPosition()).perp().dot(f.getForce(this, p));
            }
        
        Vector2D la = totalForce.divide(totMass); // linear acceleration;
        float aa = totalTorque / icm; // angular acceleration
        
        v = v.add(la.scale(dt)); // linear velocity
        w = w + aa * dt;
        
        /* Do movement */
        
        for(PointMass p : pointsMass) {
            /* foreach pointmass:
             * - compute velocity by adding linear velocity and angular velocity
             * - set the new position
             */
            Vector2D perp = cm.sub(p.getPosition()).perp();
            Vector2D vp = v.add(perp.scale(w));
            p.getPosition().set(p.getPosition().add(vp.scale(dt)));
        }
        /*
         * For the center of mass:
         * - perform movement only with the linear velocity
         */
        cm.set(cm.add(v.scale(dt)));
        
    }
    
    public Vector2D getPointVelocity(Vector2D point) {
        Vector2D perp = cm.sub(point).perp();
        return v.add(perp.scale(w)); 
    }

    
    
    public float getTotalMass() { return totMass; }
    
    public Vector2D getCenterOfMass() { return cm;}
    public double getInertia() { return icm; }
    
    public boolean addForce(Force f) { return forces.add(f); }
    public boolean removeForce(Force f) { return forces.remove(f); }
    
    public void setAngularVelocity(double w) { this.w = w; }
    public double getAngularVelocity() { return w;}
    
    public void setVelocity(Vector2D v) { this.v = v; }
    public Vector2D getVelocity() { return v; }
    
    public PointMass[] getPointsMass() { return pointsMass; }
    
    private void initialize() {
        totMass = 0;
        cm = new Vector2D();
        icm = 0;
        
        for(PointMass p : pointsMass) {
            totMass += p.getMass();
            cm = cm.add(p.getPosition().scale(p.getMass()));
        }
        
        cm = cm.divide(totMass);
        
        for(PointMass p: pointsMass) {
            double dist = cm.sub(p.getPosition()).length();
            icm += p.getMass() * dist * dist;
        }
    }

    /* Implementation of shape */
    
    @Override
    public Rectangle2D getBounds2D() {
        double x0, x1, y0, y1;
        
        x0 = y0 = Double.MAX_VALUE;
        x1 = y1 = Double.MIN_VALUE;
        
        for(PointMass p : pointsMass) {
            Vector2D pos = p.getPosition();
            
            x0 = pos.getX() < x0 ? pos.getX() : x0;
            y0 = pos.getY() < y0 ? pos.getY() : y0;
            
            x1 = pos.getX() > x1 ? pos.getX() : x1;
            y1 = pos.getY() > y1 ? pos.getY() : y1;
        }
        
        return new Rectangle2D.Double(x0, y0, x1 - x0, y1 - y0);
    }    
    
    public CollisionInfo checkCollision(RigidBody other) {
        
        CollisionInfo info = new CollisionInfo();
        Vector2D axis = null;
        
        if(!getBounds2D().intersects(other.getBounds2D())) {
            info.colliding = false;
            return info;
        }
        
        
        for(int i = 0; i < pointsMass.length; i++) {
            double min0, min1, max0, max1;
            
            min0 = min1 = Double.MAX_VALUE;
            max0 = max1 = -Double.MAX_VALUE;
            
            axis = pointsMass[(i + 1) % pointsMass.length].getPosition().sub(pointsMass[i].getPosition()).perp().normalize();
            
            
            for(int j = 0; j < pointsMass.length; j++) {
                double proj = pointsMass[j].getPosition().dot(axis);
                min0 = proj < min0 ? proj : min0;
                max0 = proj > max0 ? proj : max0;
            }
            
            for(int j = 0; j < other.pointsMass.length; j++) {
                double proj = other.pointsMass[j].getPosition().dot(axis);
                min1 = proj < min1 ? proj : min1;
                max1 = proj > max1 ? proj : max1;     
            }
            
            if((min1 >= min0 && min1 <= max0) || (max1 >= min0 && max1 <= max0))
                continue;
            else {
                info.colliding = false;
                return info;
            }
            
        }
        
        /* Bodies may be colliding */
        
        for(int i = 0; i < pointsMass.length; i++) {
            Vector2D p = pointsMass[i].getPosition();
            if(other.contains(p.x, p.y)) {
                info.colliding = true;
                info.collisionAxis = axis;
                info.collisionPoint = p;
                return info;
            }
        }
        
        /*
        for(int i = 0; i < other.pointsMass.length; i++) {
            Vector2D p = other.pointsMass[i].getPosition();
            if(contains(p.x, p.y)) {
                info.collisionPoint = p;
                return info;
            }
        }*/
        
        
        return info;      
    }
    
    @Override
    public Rectangle getBounds() {
        double x0, x1, y0, y1;
        
        x0 = y0 = Double.MAX_VALUE;
        x1 = y1 = Double.MIN_VALUE;
        
        for(PointMass p : pointsMass) {
            Vector2D pos = p.getPosition();
            
            x0 = pos.getX() < x0 ? pos.getX() : x0;
            y0 = pos.getY() < y0 ? pos.getY() : y0;
            
            x1 = pos.getX() > x1 ? pos.getX() : x1;
            y1 = pos.getY() > y1 ? pos.getY() : y1;
        }
        
        return new Rectangle((int)x0, (int)y0, 
                (int)(x1 - x0), (int)(y1 - y0));
    }    
    
    @Override
    public boolean contains(double x, double y) {
        
        if(!getBounds2D().contains(x, y))
            return false;
        
        Vector2D point = new Vector2D((float)x, (float)y);
        double angle = 0;
        for(int i = 0; i < pointsMass.length; i++) {
            Vector2D p0 = pointsMass[i].getPosition();
            Vector2D p1 = pointsMass[(i + 1) % pointsMass.length].getPosition();
            
            Vector2D v0 = p0.sub(point).normalize();
            Vector2D v1 = p1.sub(point).normalize();
            
            angle += Math.acos(v0.dot(v1));        
        }
        
        return angle >= 2 * Math.PI;
    }    

    @Override
    public boolean contains(Point2D pd) {
        return contains(pd.getX(), pd.getY());
    }
    
    @Override
    public boolean contains(double x, double y, double w, double h) {
        return contains(x, y) && contains(x + w, y) && contains(x + w, y + w) && contains(x, y + w);
    }

    @Override
    public boolean contains(Rectangle2D rd) {
        return contains(rd.getX(), rd.getY(), rd.getWidth(), rd.getHeight());
    }    

    @Override
    public boolean intersects(double x, double y, double w, double h) {
        return contains(x, y) || contains(x + w, y) || contains(x + w, y + w) || contains(x, y + w);
    }

    @Override
    public boolean intersects(Rectangle2D rd) {
        return intersects(rd.getX(), rd.getY(), rd.getWidth(), rd.getHeight());
    }

    @Override
    public PathIterator getPathIterator(AffineTransform at) {
        return new PathIteratorImpl(at);
    }

    @Override
    public PathIterator getPathIterator(AffineTransform at, double d) {
        return getPathIterator(at);
    }
    
    public static class CollisionInfo {
 
        public boolean colliding;
        public Vector2D collisionPoint, collisionAxis;        
        
        public CollisionInfo() {
            colliding = false;
            collisionPoint = null;
            collisionAxis = null;
        }
        
        
    }
    
    public class PathIteratorImpl implements PathIterator {
        
        private AffineTransform transform;
        private int currentPoint;
        private int windingRule;
        
        public PathIteratorImpl(AffineTransform at) {
            transform = at;
            currentPoint = 0;
            windingRule = PathIterator.SEG_MOVETO;
            
        }
        
        
        public PathIteratorImpl() {
            this(null);
        }
        
        public void reset() {
            currentPoint = 0;
        }

        @Override
        public int getWindingRule() {
            return PathIterator.WIND_NON_ZERO;
        }

        @Override
        public boolean isDone() {
            return currentPoint > pointsMass.length;
        }

        @Override
        public void next() {
            currentPoint++;    
        }

        @Override
        public int currentSegment(float[] floats) {
            
            if(currentPoint == pointsMass.length)
                return PathIterator.SEG_CLOSE;            
            
            floats[0] = (float)pointsMass[currentPoint].getPosition().x;
            floats[1] = (float)pointsMass[currentPoint].getPosition().y;
            
            if(transform != null)
                transform.transform(floats, 0, floats, 0, 1);
            
            return (currentPoint == 0 ? PathIterator.SEG_MOVETO : PathIterator.SEG_LINETO);
        }

        @Override
        public int currentSegment(double[] doubles) {
            
            if(currentPoint == pointsMass.length)
                return PathIterator.SEG_CLOSE;
            
            doubles[0] = pointsMass[currentPoint].getPosition().x;
            doubles[1] = pointsMass[currentPoint].getPosition().y;
            
            if(transform != null)
                transform.transform(doubles, 0, doubles, 0, 1);            
            
            return (currentPoint == 0 ? PathIterator.SEG_MOVETO : PathIterator.SEG_LINETO);
        }
        
    }
    
}
