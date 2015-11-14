/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public class Rectangle {
    
    public float x1, x2, y1, y2, witdh, height;
    
    public Rectangle(float x, float y, float width, float height) {
        x1 = x;
        x2 = x + width;
        y1 = y;
        y2 = y + height;
    }
    
    public boolean contains(float x, float y) {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }
}
