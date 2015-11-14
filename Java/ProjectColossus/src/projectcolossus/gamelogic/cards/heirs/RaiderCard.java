/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.UnitCard;
import projectcolossus.gamelogic.units.heirs.Raider;

/**
 *
 * @author Andrea
 */
public class RaiderCard extends UnitCard {

    
    public static final String NAME = "Raider";    
    public static final int MAX_MOVEMENT = 4;
    public static final int RESOURCE_COST = 3;
    public static final int POWER = 25;  
    
    private static final long serialVersionUID = 4023258309415193107L;
    
    public RaiderCard() {
        super(Constants.IDC_RAIDER, RESOURCE_COST, Constants.CK_HEIRS, POWER, MAX_MOVEMENT);
    }

    @Override
    public void play(GameMap gameMap) {
        Planet planet = gameMap.getPlayerStartingPlanet(player);
        planet.addUnit(new Raider(planet, player));
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }
    
}
