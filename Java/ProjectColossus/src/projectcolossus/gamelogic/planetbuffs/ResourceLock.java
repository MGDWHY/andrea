/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.planetbuffs;

import projectcolossus.gamelogic.PlanetBuff;
import projectcolossus.gamelogic.Player;

/**
 *
 * @author Andrea
 */
public class ResourceLock extends PlanetBuff {
    
    private static final long serialVersionUID = -1191801250284540220L;
    
    protected String name;
    protected int prevResources;
    
    public ResourceLock(int id, String name, int turnDuration, Player applier) {
        super(id, turnDuration, UPDATE_ON_BEGIN, applier);
        this.name = name;
    }
    
    @Override
    public void onDuplicate(PlanetBuff other) {
        ResourceLock rl = (ResourceLock) other;
        
        // this (de)buffs stack in duration
        planet.removeBuff(other);
        turnsLeft += rl.getTurnsLeft();  
    }    

    @Override
    public void onApply() {
        prevResources = planet.getResourcesPerTurn();
        planet.setResourcesPerTurn(0);
    }

    @Override
    public void onRemove() {
        planet.setResourcesPerTurn(prevResources);
    }

    @Override
    public void onTurnBegin(Player currentPlayer) {}

    @Override
    public void onTurnEnd(Player currentPlayer) {}

    @Override
    public String getName() {
        return name;
    }
    
}
