/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.UnitCard;
import projectcolossus.gamelogic.units.heirs.MechaSquad;

/**
 *
 * @author Andrea
 * This ship can move through enemy planets
 */
public class MechaSquadCard extends UnitCard {
    
    public static final String NAME = "Mecha Squad";    
    public static final int MAX_MOVEMENT = 3;
    public static final int RESOURCE_COST = 6;
    public static final int POWER = 40;    
    
    private static final long serialVersionUID = 4090455924370427769L;
    
    public MechaSquadCard() {
        super(Constants.IDC_MECHA_SQUAD, RESOURCE_COST, Constants.CK_HEIRS, POWER, MAX_MOVEMENT);
    }

    @Override
    public void play(GameMap gameMap) {
        Planet planet = gameMap.getPlayerStartingPlanet(player);
        planet.addUnit(new MechaSquad(planet, player));
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }
    
    
}
