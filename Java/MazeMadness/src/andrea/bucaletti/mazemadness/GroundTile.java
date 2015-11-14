/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.mazemadness;

/**
 *
 * @author Andrea
 */
public class GroundTile extends Tile {
    
    protected StraightLine2f collisionLine;
    
    public GroundTile(Point2i position, Point2i p1, Point2i p2) {
        super(position);
        collisionLine = new StraightLine2f(
                new Point2f(p1.x + position.x, p1.y + position.y),
                new Point2f(p2.x + position.x, p2.y + position.y)
        );
    }

    @Override
    public boolean isCollideable() {
        return true;
    }

    @Override
    public boolean handleCollision(Player player) {
        return collisionLine.distance(player.getPosition()) < player.getRadius();
    }
}
