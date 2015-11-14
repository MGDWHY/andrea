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
public abstract class FreeCard extends Card {
    private static final long serialVersionUID = -3450089951493391444L;

    
    public FreeCard(int id, int resourceCost, int kind) {
        super(id, resourceCost, kind);
    }

    @Override
    public final boolean canPlayRegardless() {
        return true;
    }

    @Override
    public final boolean canPlayOnPlanet(GameMap gameMap, Planet planet) {
        return false;
    }

    @Override
    public final boolean canPlayOnUnit(Unit unit) {
        return false;
    }

    @Override
    public final void playOnPlanet(GameMap gameMap, Planet planet) {
        throw new UnsupportedOperationException("This card can't be played on a planet");
    }
    
}
