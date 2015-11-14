/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.planetbuffs;

import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.commmon.OrbitalDefenceCard;

/**
 *
 * @author Andrea
 */
public class OrbitalDefence extends AlterPlanetConquestPoints {
    
    private static final long serialVersionUID = 1217279890624848489L;
    
    public OrbitalDefence(int id, Player applier) {
        super(id, OrbitalDefenceCard.TURN_DURATION, applier, OrbitalDefenceCard.NAME, OrbitalDefenceCard.CONQUEST_POINT_INCREASE);
    }
    
    @Override
    public void onTurnEnd(Player currentPlayer) {
        if(!applier.equals(planet.getOwner()))
            markForRemoval();
    }    
    
}
