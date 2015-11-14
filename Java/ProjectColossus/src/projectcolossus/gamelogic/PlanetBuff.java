/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.Serializable;

/**
 * A class representing a an effect (buff/debuff) on a planet. Subclasses
 * must extend this class specifying the behavior through the abstract methods
 * onApply(), onRemove(), onTurnBegin(), onTurnEnd(), onDuplicate
 * @author Andrea
 */
public abstract class PlanetBuff implements Serializable {

    public static final int UPDATE_ON_END = 0;
    public static final int UPDATE_ON_BEGIN = 1;
       
    private static final long serialVersionUID = -2099081783377318905L;
    
    protected Player applier;
    
    protected int id;
    protected Planet planet;
    protected int turnDuration; // -1 = infinite
    
    protected int turnsLeft;
    protected boolean markForRemoval;
    
    protected int turnUpdate;
    
    public PlanetBuff(int id, int turnDuration, int turnUpdate, Player applier) {
        this.id = id;
        this.turnDuration = this.turnsLeft = turnDuration;
        this.applier = applier;
        this.turnUpdate = turnUpdate;
        this.markForRemoval = false;
    }
    
    public void markForRemoval() { this.markForRemoval = true; }
    
    public int getTurnsLeft() {
        return turnsLeft;
    }
    
    public void setPlanet(Planet p) { this.planet = p; }
    public Planet getPlanet() { return this.planet; }
    
    /**
     * Called by the game engine. If this buff is an UPDATE_ON_BEGIN buff,
     * a turn will be consumed on the beginning of each turn of the applier
     * @param currentPlayer 
     */
    public void beginTurn(Player currentPlayer) {
        if(turnUpdate == UPDATE_ON_BEGIN)
            updateTurn(currentPlayer);
    }
    
    /**
     * Called by the game engine. If this buff is an UPDATE_ON_END buff,
     * a turn will be consumed on the end of each turn of the applier
     * @param currentPlayer 
     */
    public void endTurn(Player currentPlayer) {
        if(turnUpdate == UPDATE_ON_END)
            updateTurn(currentPlayer);
    }
    
    public Player getApplier() { return applier; }
    
    public final int getID() {
        return id;
    }
    
    
    /**
     * Tests if the this buff has similar effect to onotherbuff
     * @param other the other buff
     * @return true if the buff have the same type of effect (ie they are the same class)
     */
    public boolean sameAs(PlanetBuff other) {
        return this.getClass() == other.getClass();
    }
    
    /**
     * Called by the game engine when a buff is about to be applied (Planet.applyBuff()). Here subclasses
     * can implement different behaviors, like staking buffs of the same class or replace them. A buff is considerd
     * a duplicate of this buff when this statement is true:
     *      this.sameAs(other)
     * @param other 
     */
    public abstract void onDuplicate(PlanetBuff other);
    
    /**
     * Called by the planet before adding the buff to the planet. Note that the setPlanet methoed
     * has been also called and filled with the planet object
     */
    public abstract void onApply();
    
    /**
     * Called when this buff is about to be removed from the planet
     */
    public abstract void onRemove();
    
    /**
     * Called when a turn begins
     * @param currentPlayer The current player
     */
    public abstract void onTurnBegin(Player currentPlayer);
    
    /**
     * Called when a turn ends
     * @param currentPlayer 
     */
    public abstract void onTurnEnd(Player currentPlayer);  
    
    public abstract String getName();
    
    private void updateTurn(Player currentPlayer) {
        if(markForRemoval) {
            planet.removeBuff(this);
            return;
        }
        
        if(turnDuration == -1)
            return;
        
        if(currentPlayer.equals(applier) && --turnsLeft == 0)
            planet.removeBuff(this);        
    }    
    
}
