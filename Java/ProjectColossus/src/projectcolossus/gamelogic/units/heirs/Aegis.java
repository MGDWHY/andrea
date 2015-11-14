/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units.heirs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.AegisCard;
import projectcolossus.gamelogic.planetbuffs.FlyingAegis;
import projectcolossus.gamelogic.units.RegularUnit;

/**
 *
 * @author Andrea
 */
public class Aegis extends RegularUnit {
    
    private static final long serialVersionUID = -6773191598672607347L;
    
    protected FlyingAegis flyingAegis;
    
    public Aegis(Planet planet, Player player) {
        super(Constants.IDC_AEGIS, planet, player, AegisCard.POWER, AegisCard.MAX_MOVEMENT);
        
    }
    
    @Override
    public boolean onKill() {
        planet.removeBuff(flyingAegis);
        return true;
    }
    
    @Override
    public boolean onCreate() {
        flyingAegis = new FlyingAegis(Constants.IDB_FLYING_AEGIS, this);
        planet.applyBuff(flyingAegis);
        return true;
    }    
    
    @Override
    public boolean onMove(Planet origin, Planet destination) {
        origin.removeBuff(flyingAegis);
        destination.applyBuff(flyingAegis);
        return true;
    }
    
    @Override
    public String getName() {
        return AegisCard.NAME;
    }
    
}
