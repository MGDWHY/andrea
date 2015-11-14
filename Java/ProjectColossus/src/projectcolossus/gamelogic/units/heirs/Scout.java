/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.ScoutCard;
import projectcolossus.gamelogic.units.RegularUnit;

/**
 *
 * @author Andrea
 */
public class Scout extends RegularUnit {
    
    private static final long serialVersionUID = -8701609781553380760L;
    
    public Scout(Planet planet, Player player) {
        super(Constants.IDC_SCOUT, planet, player, ScoutCard.POWER, ScoutCard.MAX_MOVEMENT);
    }

    @Override
    public String getName() {
        return ScoutCard.NAME;
    }
    
}
