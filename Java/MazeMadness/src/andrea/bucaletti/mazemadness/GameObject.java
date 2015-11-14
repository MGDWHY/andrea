/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public abstract class GameObject {
    public abstract Rectangle getBounds();
    public abstract boolean isCollideable();
    public abstract boolean handleCollision(Player player);
}
