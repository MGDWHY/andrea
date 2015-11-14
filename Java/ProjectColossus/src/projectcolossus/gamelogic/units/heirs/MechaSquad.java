/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.MechaSquadCard;
import projectcolossus.gamelogic.units.RaiderUnit;

/**
 *
 * @author Andrea
 */
public class MechaSquad extends RaiderUnit {
    
    private static final long serialVersionUID = -5575259158231983829L;

    public MechaSquad(Planet planet, Player player) {
        super(Constants.IDC_MECHA_SQUAD, planet, player, MechaSquadCard.POWER, MechaSquadCard.MAX_MOVEMENT);
    }
    
    @Override
    public String getName() {
        return MechaSquadCard.NAME;
    }
    
}
