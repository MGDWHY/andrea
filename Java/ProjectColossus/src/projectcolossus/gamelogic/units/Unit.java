/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.units;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Comparator;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.ai.RouteFinder;

/**
 *
 * @author Andrea
 * Base class for game units.
 */
public abstract class Unit implements Serializable {
    
    private static int nextID = 1;
    
    private static final long serialVersionUID = -3967808337639730817L;
    
    public static final int newID() {
        return(nextID++);
    }
    
    protected int cardRefID;
    
    protected int id;
    
    protected int power;
    
    protected int maxMovement;
    
    protected int movement;
    
    protected Player player;
    
    protected Planet planet;
    
    public Unit(int cardRefID, Planet planet, Player player, int power, int maxMovement) {
        this.id = newID();
        this.cardRefID = cardRefID;
        this.player = player;
        this.power = power;
        this.maxMovement = maxMovement;
        this.planet = planet;
        
        player.addUnit(this);
        onCreate();
        resetMovement();
    }
    
    public final int getID() {
        return id;
    }   
    
    public final int getRefCardID() {
        return cardRefID;
    }
    
    public final Player getPlayer() {
        return player;
    }
    
    public final void setPlanet(Planet planet) {
        this.planet = planet;
    }
    
    public final Planet getPlanet() {
        return planet;
    }
    
    public final boolean kill() {
        
        onKill();
        
        if(planet != null)
            planet.removeUnit(this);
        
        if(player != null)
            player.removeUnit(this);
        
        return true;
    }

    public final boolean moveTo(GameMap map, Planet origin, Planet destination) {
        
        if(movement == 0)
            return false;    
        else if(!origin.getAllowMoveOut())
            return false;     
        else if(!destination.getAllowMoveIn())
            return false;      
        else {
            RouteFinder finder = new RouteFinder(map);
            
            ArrayList<RouteFinder.State> routes = finder.findRoutes(origin, destination, movement);
            
            for(RouteFinder.State s : routes) {
                boolean result = true;
                ArrayList<Planet> path = s.getPath();
                
                for(int i = 0; i < path.size() - 1; i++)
                    result = result & canMove(map, path.get(i), path.get(i + 1));
                
                if(result) {                  
                    movement -= (path.size() - 1);
                    if(origin.removeUnit(this)) {
                        destination.addUnit(this);
                        onMove(origin, destination);
                        return true;
                    } else
                        return false;
                }
            }
            
            return false;    
        }
    }
    
    public final void resetMovement() {
        this.movement = maxMovement;
    }
    
    public String toString() {
        return getName() + " - P:" + getPower() + " M:" + movement;
    }
    
    public final int getMovement() {
        return movement;
    }
    public final int getPower() {
        return power;
    }
    /**
     * Executed when this unit is created. Subclasses should override this if needed
     */
    public boolean onCreate() { return true; }
    
    /**
     * Returns true if the unit can move from a planet to another. Subclasses should override this if needed
     * @param map The game map
     * @param origin The planet the unit is moving from
     * @param destination The destination planet
     * @return true if the unit can move, false otherwise
     */
    public boolean canMove(GameMap map, Planet origin, Planet destination) { return true; }
    
    /**
     * Executed when a unit is being moved form a planet to another. Subclasses should override this if needed
     * @param origin The planet the unit is being moved from 
     * @param destination The destination planet
     */
    public boolean onMove(Planet origin, Planet destination) { return true; }
    
    /**
     * Executed when this unit is about to be killed. Subclasses should override it if needed
     * @return 
     */
    public boolean onKill() { return true; }
    
    public abstract String getName();
    
    /**
     * A unit is equal to another unit if they have the same ID, so if they are physically the same unit
     * @param x the other unit
     * @return 
     */
    @Override
    public final boolean equals(Object x) {   
        if(!(x instanceof Unit))
            return false;
        
        Unit other = (Unit)x;     
        return this.id == other.id;
    }

    @Override
    public final int hashCode() {
        int hash = 7;
        hash = 97 * hash + this.id;
        return hash;
    }
    /**
     * This comparator can be used to order the units by their power
     */
    public static class PowerComparator implements Comparator<Unit> {
        @Override
        public int compare(Unit t1, Unit t2) {
            if(t1.getPower() < t2.getPower())
                return -1;
            else if(t1.getPower() > t2.getPower())
                return 1;
            else
                return 0;
        }
    }
}
