/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public class StraightLine2f {
    
    private float m, q;
      
    public StraightLine2f(Point2f p1, Point2f p2) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
        q = p1.y - m * p1.x;
    }
    
    public float distance(Point2f p) {
        return (float)(Math.abs(p.y - p.x * m - q)) / (float) Math.sqrt(1 + m * m);
    }
}
