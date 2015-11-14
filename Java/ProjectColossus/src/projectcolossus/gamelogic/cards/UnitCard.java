/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards;

import projectcolossus.gamelogic.GameMap;

/**
 *
 * @author Andrea
 */
public abstract class UnitCard extends FreeCard {
    
    private static final long serialVersionUID = -7789983713620945559L;
    
    protected int power;
    protected int maxMovement;
    
    public UnitCard(int id, int resourceCost, int kind, int power, int maxMovement) {
        super(id, resourceCost, kind);
        this.power = power;
        this.maxMovement = maxMovement;
    }

    public int getPower() {
        return power;
    }
    
    public int getMovement() {
        return maxMovement;
    } 
    
    @Override
    public abstract void play(GameMap gameMap);

    @Override
    public abstract String getName();
    

    
}
