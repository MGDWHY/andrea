/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.cards.FreeCard;

/**
 *
 * @author Andrea
 * Gives 2 resources to the player for the current turn
 */
public class PlanetaryExplotationCard extends FreeCard {
    
    public static final String NAME = "Planetary Exploitation";
    public static final int RESOURCE_COST = 0;
    
    public static final int RESOURCE_GIVEN = 2;
    
    private static final long serialVersionUID = 6931274451417883112L;
    
    public PlanetaryExplotationCard() {
        super(Constants.IDC_PLANETARY_EXPLOITATION, RESOURCE_COST, Constants.CK_COMMON);
    }

    @Override
    public void play(GameMap gameMap) {
        player.addResources(RESOURCE_GIVEN);
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }  
    
}
