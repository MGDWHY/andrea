/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.UnitCard;
import projectcolossus.gamelogic.units.heirs.Scout;

/**
 *
 * @author Andrea
 */
public class ScoutCard extends UnitCard {
    
    public static final String NAME = "Scout";
    public static final int MAX_MOVEMENT = 2;
    public static final int RESOURCE_COST = 1;
    public static final int POWER = 20;    
    
    private static final long serialVersionUID = -1449649910529486096L;
    
    public ScoutCard() {
        super(Constants.IDC_SCOUT, RESOURCE_COST, Constants.CK_HEIRS, POWER, MAX_MOVEMENT);
    }

    @Override
    public void play(GameMap gameMap) {
        Planet planet = gameMap.getPlayerStartingPlanet(player);
        planet.addUnit(new Scout(planet, player));
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }   
    
}
