/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public class EmptyTile extends Tile {
    
    public EmptyTile(Point2i position) {
        super(position);
    }
    
    public EmptyTile(int x, int y) {
        super(x, y);
    }

    @Override
    public boolean isCollideable() {
        return false;
    }

    @Override
    public boolean handleCollision(Player player) {
        throw new UnsupportedOperationException("This tile is empty");
    }
    
}
