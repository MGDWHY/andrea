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
public class AlterPlanetConquestPoints extends PlanetBuff {
    
    public static final String NAME = "Orbital Defence";
    
    private static final long serialVersionUID = -5833205377439966579L;
    
    protected String name;
    
    protected int amount;
    
    public AlterPlanetConquestPoints(int id, int duration, Player applier, String name, int amount) {
        super(id, duration, UPDATE_ON_END, applier);
        this.name = name;
        this.amount = amount;
    }
    
    @Override
    public void onDuplicate(PlanetBuff other) {
        // Do nothing.. these buffs are cumulative
    }    

    @Override
    public void onApply() {
        planet.getConquestData().addToMaxValue(amount);
    }

    @Override
    public void onRemove() {
        planet.getConquestData().addToMaxValue(-amount);
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
