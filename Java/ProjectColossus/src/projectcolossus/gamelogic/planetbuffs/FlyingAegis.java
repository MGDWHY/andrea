/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.planetbuffs;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.PlanetBuff;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.heirs.AegisCard;
import projectcolossus.gamelogic.units.heirs.Aegis;

/**
 *
 * @author Andrea
 */
public class FlyingAegis extends PlanetBuff {
    
    private static final long serialVersionUID = 5870023153317758498L;
    
    protected Planet startPlanet;
    protected Player startOwner;
    
    protected Aegis aegis;
    
    public FlyingAegis(int id, Aegis aegis) {
        super(id, -1, UPDATE_ON_END, aegis.getPlayer());
        this.aegis = aegis;
    }
    
    @Override
    public void onDuplicate(PlanetBuff other) {
        // Do nothing.. more than 1 Aegis can be on the same planet at the same time
    }    
    

    @Override
    public void onApply() {
        startPlanet = planet;
        startOwner = planet.getOwner();
    }

    @Override
    public void onRemove() {}

    @Override
    public void onTurnBegin(Player currentPlayer) {
        startPlanet = planet;
        startOwner = planet.getOwner();
    }

    @Override
    public void onTurnEnd(Player currentPlayer) {
        Planet finalPlanet = planet;
        Player finalOwner = planet.getOwner();
        
        if(finalPlanet.isOwned()) {
            if(!finalOwner.equals(startOwner) && finalOwner.equals(aegis.getPlayer())) {
                planet.applyBuff(new ResourceLock(Constants.IDB_THERMONUCLEAR_BOMBING, AegisCard.BUFF_AEGIS_DESTRUCTION, 1, applier));
            }
        }
    }

    @Override
    public String getName() {
        return AegisCard.BUFF_AEGIS_PRESENCE;
    }

    
}
