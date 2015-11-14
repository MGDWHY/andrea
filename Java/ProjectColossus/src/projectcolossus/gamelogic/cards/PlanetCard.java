/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards;

import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.units.Unit;

/**
 *
 * @author Andrea
 */
public abstract class PlanetCard extends Card {
    private static final long serialVersionUID = -570494167411739251L;

    
    protected boolean friendlyPlanet, neutralPlanet, enemyPlanet;
    
    public PlanetCard(int id, int resourceCost, int kind, boolean friendlyPlanet, boolean neutralPlanet, boolean enemyPlanet) {
        super(id, resourceCost, kind);
        this.friendlyPlanet = friendlyPlanet;
        this.neutralPlanet = neutralPlanet;
        this.enemyPlanet = enemyPlanet;
    }

    @Override
    public final boolean canPlayRegardless() {
        return false;
    }

    @Override
    public final boolean canPlayOnPlanet(GameMap gameMap, Planet planet) {
        if(!planet.isOwned() && neutralPlanet)
            return true;
        
        if(planet.getOwner().equals(player) && friendlyPlanet)
            return true;
        
        if(!planet.getOwner().equals(player) && enemyPlanet)
            return true;
        
        return false;
    }

    @Override
    public final boolean canPlayOnUnit(Unit unit) {
        return false;
    }

    @Override
    public final void play(GameMap gameMap) {
        throw new UnsupportedOperationException("This card must be played on a planet");
    }
    
}
