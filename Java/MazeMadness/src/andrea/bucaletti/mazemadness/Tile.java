/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 * A static tile of the game level
 */
public abstract class Tile extends GameObject {
    /**
     * x and y are the positions in the tilemap. The size of a tile is 1
     */
    protected Point2i position;
    
    protected Rectangle bounds;
    
    public Tile(int x, int y) {
        bounds = new Rectangle(x, y, 1, 1);
        position = new Point2i(x, y);
    }
    
    public Tile(Point2i position) {
        bounds = new Rectangle(position.x, position.y, 1, 1);
        this.position = position;
    }
    
    public Point2i getPosition() { return position; }

    @Override
    public Rectangle getBounds() {
        return bounds;
    }
}
