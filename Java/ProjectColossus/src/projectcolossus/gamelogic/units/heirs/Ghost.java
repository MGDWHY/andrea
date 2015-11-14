/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.GhostCard;
import projectcolossus.gamelogic.units.RegularUnit;

/**
 *
 * @author Andrea
 */
public class Ghost extends RegularUnit {
    
    private static final long serialVersionUID = 3777170055044574000L;
    
    public Ghost(Planet planet, Player player) {
        super(Constants.IDC_GHOST, planet, player, GhostCard.POWER, GhostCard.MAX_MOVEMENT);
    }

    @Override
    public String getName() {
        return GhostCard.NAME;
    }
    
}
