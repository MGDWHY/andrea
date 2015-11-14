/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.planetbuffs.MovementLock;

/**
 *
 * @author Andrea
 * Units cannot enter or leave this planet
 */
public class PlanetaryCageCard extends PlanetCard {

    public static final String NAME = "Planetary Cage";
    public static final int TURN_DURATION = 2;
    public static final int RESOURCE_COST = 2;
    
    private static final long serialVersionUID = 7928046325292942282L;

    public PlanetaryCageCard() {
        super(Constants.IDC_PLANETARY_CAGE, RESOURCE_COST, Constants.CK_COMMON, true, true, true);
    }    
    
    @Override
    public void playOnPlanet(GameMap gameMap, Planet planet) {
        planet.applyBuff(new MovementLock(Constants.IDB_PLANETARY_CAGE, TURN_DURATION, player, false, false, NAME));
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }
  
}
