/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.UnitCard;
import projectcolossus.gamelogic.units.heirs.Ghost;

/**
 *
 * @author Andrea
 */
public class GhostCard extends UnitCard {

    public static final String NAME = "Ghost";  
    
    public static final int MAX_MOVEMENT = 3;
    public static final int RESOURCE_COST = 4;
    public static final int POWER = 35;    
    
    private static final long serialVersionUID = -4845183766648674698L;
    
    public GhostCard() {
        super(Constants.IDC_GHOST, RESOURCE_COST, Constants.CK_HEIRS, POWER, MAX_MOVEMENT);
    }

    @Override
    public void play(GameMap gameMap) {
        Planet planet = gameMap.getPlayerStartingPlanet(player);
        planet.addUnit(new Ghost(planet, player));
        played();    
    }

    @Override
    public String getName() {
        return NAME;
    }     
    
}
