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
 * These units can move freely between enemy planets
 */
public abstract class RaiderUnit extends Unit {
    
    private static final long serialVersionUID = 5724409884343973321L;
    
    public RaiderUnit(int cardRefID, Planet planet, Player player, int power, int maxMovement) {
        super(cardRefID, planet, player, power, maxMovement);
    }
    
    @Override
    public final boolean canMove(GameMap map, Planet origin, Planet destination) {
        return true;
    }
}
