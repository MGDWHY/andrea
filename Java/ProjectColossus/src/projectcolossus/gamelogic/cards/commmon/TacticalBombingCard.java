/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.planetbuffs.AlterPlanetConquestPoints;

/**
 *
 * @author Andrea
 * This card decreases the points needed to conquest a planet
 * by 50% for 1 turn; stacks up to 2 times;
 */
public class TacticalBombingCard extends PlanetCard {
    
    public static final String NAME = "Tactical Bombing";
    public static final int RESOURCE_COST = 0;
    public static final int TURN_DURATION = 1;
    
    private static final long serialVersionUID = -1412400960396244717L;
    
    public TacticalBombingCard() {
        super(Constants.IDC_TACTICAL_BOMBING, RESOURCE_COST, Constants.CK_COMMON, false, true, true);
    }

    @Override
    public void playOnPlanet(GameMap gameMap, Planet planet) {
        int amount = planet.getConquestData().getMaxValue() / 2;
        planet.applyBuff(new AlterPlanetConquestPoints(Constants.IDB_TACTICAL_BOMBING, TURN_DURATION, player, NAME, -amount));
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }
}
