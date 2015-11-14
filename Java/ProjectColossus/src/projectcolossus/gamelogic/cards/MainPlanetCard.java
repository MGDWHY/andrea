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
public abstract class MainPlanetCard extends Card {
    private static final long serialVersionUID = 3303080495760995858L;

    
    public MainPlanetCard(int id, int resourceCost, int kind) {
        super(id, resourceCost, kind);
    }

    @Override
    public final boolean canPlayRegardless() {
        return false;
    }

    @Override
    public final boolean canPlayOnPlanet(GameMap gameMap, Planet planet) {
        if(planet.equals(gameMap.getPlayerStartingPlanet(this.player)))
            return true;
        else
            return false;
    }

    @Override
    public final boolean canPlayOnUnit(Unit unit) {
        return false;
    }

    @Override
    public final void play(GameMap gameMap) {
        throw new UnsupportedOperationException("This card must be played in the main planet");
    }
    
}
