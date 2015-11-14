/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.units;

import projectconquest.data.map.Location;

/**
 *
 * @author Andrea
 */
public abstract class Unit {
    
    /* UNIT TEMPLATES IDs */
    public static final int TID_SOLDIER = 1;
    
    protected int templateID;
    
    protected int id;
    
    protected int attack, hitPoints, movement;
    
    protected int currentHitPoints, movementLeft;
    
    protected int locationIndex;
    
    public Unit(int templateID, int id, int attack, int hitPoints, int movement) {
        this.templateID = templateID;
        this.id = id;
        this.attack = attack;
        this.hitPoints = this.currentHitPoints = hitPoints;
        this.movement = this.movementLeft = movement;
    }
    
    public void setLocationIndex(int locIndex) { this.locationIndex = locIndex; }
    
    public int getLocationIndex() { return this.locationIndex; }
    
    public void resetMovement() { movementLeft = movement; }
    
    public void heal(int amount) {
        currentHitPoints += amount;
        
        if(currentHitPoints > hitPoints) 
            currentHitPoints = hitPoints;
    }
    
    public void doDamage(int amount) {
        currentHitPoints -= amount;
        
        if(currentHitPoints < 0)
            currentHitPoints = 0;
    }
    
    public boolean isDead() {return currentHitPoints == 0;}
    public boolean isAlive() { return currentHitPoints > 0;}
}
