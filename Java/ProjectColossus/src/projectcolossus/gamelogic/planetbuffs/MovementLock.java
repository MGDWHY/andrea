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
public class MovementLock extends PlanetBuff {
    
    private static final long serialVersionUID = 5740828379444415046L;
    
    protected String name;
    
    protected boolean allowMoveIn, allowMoveOut;
    
    public MovementLock(int id, int turnDuration, Player applier, boolean allowMoveIn, boolean allowMoveOut, String name) {
        super(id, turnDuration, UPDATE_ON_END, applier);
        this.allowMoveIn = allowMoveIn;
        this.allowMoveOut = allowMoveOut;
        this.name = name;
    }
    
    @Override
    public void onDuplicate(PlanetBuff other) {
        // Do nothing... This buffs are cumulative
    }    

    @Override
    public void onApply() {
        planet.setAllowMoveIn(allowMoveIn);
        planet.setAllowMoveOut(allowMoveOut);
    }

    @Override
    public void onRemove() {
        planet.setAllowMoveOut(true);
        planet.setAllowMoveIn(true);
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
