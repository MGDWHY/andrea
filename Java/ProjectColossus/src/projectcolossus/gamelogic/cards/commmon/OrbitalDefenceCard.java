/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.planetbuffs.OrbitalDefence;

/**
 *
 * @author Andrea
 * Increase points to conquest a friendly planet by 100. The orbital defence
 * will get destroyed if the planet is lost.
 */
public class OrbitalDefenceCard extends PlanetCard {

    
    public static final String NAME = "Orbital Defence";
    public static final int RESOURCE_COST = 2;
    public static final int CONQUEST_POINT_INCREASE = 100;
    public static final int TURN_DURATION = -1;
    
    private static final long serialVersionUID = -4820057417936707701L;
    
    
    public OrbitalDefenceCard() {
        super(Constants.IDC_ORBITAL_DEFENCE, RESOURCE_COST, Constants.CK_COMMON, true, false, false);
    }

    @Override
    public void playOnPlanet(GameMap gameMap, Planet planet) {
        planet.applyBuff(new OrbitalDefence(Constants.IDB_ORBITAL_DEFENCE, player));
        played();
    }
    

    @Override
    public String getName() {
        return NAME;
    }
    
}
