/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.UnitCard;
import projectcolossus.gamelogic.units.heirs.Aegis;

/**
 *
 * @author Andrea
 * If this ship conquest a planet, that will give no resources for the next 2 turns
 */
public class AegisCard extends UnitCard {
    
    public static final String NAME = "Aegis";    
    
    public static final String BUFF_AEGIS_PRESENCE = "Aegis Presence";
    public static final String BUFF_AEGIS_DESTRUCTION = "Aegis Destruction";
    
    public static final int MAX_MOVEMENT = 1;
    public static final int RESOURCE_COST = 8;
    public static final int POWER = 100;
    
    private static final long serialVersionUID = 3060636563644835309L;
    
    public AegisCard() {
        super(Constants.IDC_AEGIS, RESOURCE_COST, Constants.CK_HEIRS, POWER, MAX_MOVEMENT);
    }

    @Override
    public void play(GameMap gameMap) {
        Planet planet = gameMap.getPlayerStartingPlanet(player);
        planet.addUnit(new Aegis(planet, player));
        played();   
    }

    @Override
    public String getName() {
        return NAME;
    } 
    
}
