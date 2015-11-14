/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.planetbuffs.ResourceLock;
import projectcolossus.gamelogic.units.Unit;

/**
 *
 * @author Andrea
 * Destroys all the units on a planet, but all the player's planets
 * won't give any resources the next turn
 */
public class NuclearMissile extends PlanetCard {
    
    public static final int RESOURCE_COST = 0;
    public static final int RESOURCE_LOCK_TURN_DURATION = 1;
    
    public static final String NAME = "Nuclear Missile";
    public static final String BUFF_RESOURCE_DEPLETION = "Resource Depletion";
    
    private static final long serialVersionUID = 6931922983457821626L;
    
    public NuclearMissile() {
        super(Constants.IDC_NUCLEAR_MISSILE, RESOURCE_COST, Constants.CK_COMMON, true, true, true);
    }
    

    @Override
    public void playOnPlanet(GameMap gameMap, Planet planet) {
        
        planet.getUnits().lock();
        
        for(Unit u : planet.getUnits())
            u.kill();
        
        planet.getUnits().unlock();
        
        for(Planet p : gameMap.getPlanets())
            if(p.isOwned() && p.getOwner().equals(player))
                p.applyBuff(new ResourceLock(Constants.IDB_RESOURCE_DEPLETION, BUFF_RESOURCE_DEPLETION, RESOURCE_LOCK_TURN_DURATION, player));
        
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }  
    
}
