/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.RaiderCard;
import projectcolossus.gamelogic.units.RegularUnit;

/**
 *
 * @author Andrea
 */
public class Raider extends RegularUnit {
    
    private static final long serialVersionUID = 2028041211984906318L;

    public Raider(Planet planet, Player player) {
        super(Constants.IDC_RAIDER, planet, player, RaiderCard.POWER, RaiderCard.MAX_MOVEMENT);
    }
    
    @Override
    public String getName() {
        return RaiderCard.NAME;
    }
    
}
