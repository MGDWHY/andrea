/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.HunterCard;
import projectcolossus.gamelogic.units.RegularUnit;

/**
 *
 * @author Andrea
 */
public class Hunter extends RegularUnit {
    
    private static final long serialVersionUID = 1833737482127133075L;
    
    public Hunter(Planet planet, Player player) {
        super(Constants.IDC_HUNTER, planet, player, HunterCard.POWER, HunterCard.MAX_MOVEMENT);
    }

    @Override
    public String getName() {
        return HunterCard.NAME;
    }
}
