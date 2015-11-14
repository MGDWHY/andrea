/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units;

import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;

/**
 *
 * @author Andrea
 * Regular units can move only between allied or neutral planets.
 * It can move to(attack) an enemy planet, but cannot move between
 * enemy planets. 
 */
public abstract class RegularUnit extends Unit {
    
    private static final long serialVersionUID = 1828500057627077549L;
    
    public RegularUnit(int cardRefID, Planet planet, Player player, int power, int maxMovement) {
        super(cardRefID, planet, player, power, maxMovement);
    }
      

    @Override
    public final boolean canMove(GameMap map, Planet origin, Planet destination) {
        if(origin.isOwned() && destination.isOwned() && 
            !origin.getOwner().equals(this.player) &&
            !destination.getOwner().equals(this.player)) // the 2 planets are owned by enemy players, so can't move
            return false;
        else // can move 
            return true;
    }
    
    @Override
    public boolean onMove(Planet origin, Planet destination) {
        return true;
    }
    
}
