/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.Serializable;
import java.util.ArrayList;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.util.LockableArrayList;
/**
 *
 * @author Andrea
 */

/*
 * TODO Implementare uguaglianza tramite ID e non name
 */
public class Planet implements Serializable {
    
    public static final float PLANET_MIN_RADIUS = 5;
    
    public static final float DEFAULT_PLANET_RADIUS = 10;
    public static final float DEFAULT_ROTATION_SPEED = (float)20;
    public static final int DEFAULT_APPEARANCE = 0;
    public static final int DEFAULT_RESOURCES_PER_TURN = 1;
    
    private static int nextID = 1;
    private static final long serialVersionUID = -3138168920148068466L;
    
    protected int id;
    protected int appearance;
    protected float rotationalSpeed;
    protected int resourcesPerTurn;
    protected float radius;
    protected boolean allowMoveIn, allowMoveOut;
    protected String name;
    protected Vec2f position;
    protected Player owner;
    protected LockableArrayList<Unit> units;
    protected LockableArrayList<PlanetBuff> buffs;
    protected GameData.ConquestData conquestData;
    
    public static int newID() { return (nextID++); }
    
    public Planet(String name, Vec2f position) {
        this.id = newID();
        this.name = name;
        this.position = position;
        this.appearance = DEFAULT_APPEARANCE;
        this.rotationalSpeed = DEFAULT_ROTATION_SPEED;
        this.radius = DEFAULT_PLANET_RADIUS;
        this.resourcesPerTurn = DEFAULT_RESOURCES_PER_TURN;
        this.allowMoveIn = this.allowMoveOut = true;
        this.units = new LockableArrayList<Unit>();
        this.buffs = new LockableArrayList<PlanetBuff>();
    }
    
    public int getID() { return id; }
    
    public int getAppearance() { return appearance; }
    public void setAppearance(int appearance) { this.appearance = appearance; }
    
    public float getRotationalSpeed() { return rotationalSpeed; }
    public void setRotationalSpeed(float rotSpeed) { this.rotationalSpeed = rotSpeed; }
    
    public void setAllowMoveIn(boolean val) { this.allowMoveIn = val; }
    public boolean getAllowMoveIn() { return this.allowMoveIn; }
    
    public void setAllowMoveOut(boolean val) { this.allowMoveOut = val; }
    public boolean getAllowMoveOut() { return this.allowMoveOut; }
    
    public void setConquestData(GameData.ConquestData data) { this.conquestData = data; }
    public GameData.ConquestData getConquestData() { return conquestData; }
    
    public void setResourcesPerTurn(int r) { resourcesPerTurn = r; }
    public int getResourcesPerTurn() { return resourcesPerTurn; }
    
    public boolean applyBuff(PlanetBuff buff) {
        
        buff.setPlanet(this);
        
        buffs.lock();
        
        for(PlanetBuff b : buffs)
            if(buff.sameAs(b))
                buff.onDuplicate(b);
        
        buffs.unlock();
        
        
        buff.onApply();
        return buffs.add(buff);
    }
    
    public boolean removeBuff(PlanetBuff buff) {
        buff.onRemove();
        buff.setPlanet(null);
        return buffs.remove(buff);
    }
    
    public LockableArrayList<PlanetBuff> getBuffs() {
        return buffs;
    }
    
    public boolean addUnit(Unit unit) {
        unit.setPlanet(this);
        return units.add(unit);
    }
    public boolean removeUnit(Unit unit) {
        if(unit.getPlanet().equals(this))
            unit.setPlanet(null);
        return units.remove(unit);
    }
    public LockableArrayList<Unit> getUnits() {
        return units;
    }   
    
    public ArrayList<Unit> getPlayerUnits(Player player) {
        ArrayList<Unit> punits = new ArrayList<Unit>();
        
        for(int i = 0; i < units.size(); i++) 
            if(units.get(i).getPlayer().equals(player))
                punits.add(units.get(i));
        
        return punits;
    }
    
    public int getPlayerPower(Player player) {
        int result = 0;
        ArrayList<Unit> punits = getPlayerUnits(player);
        for(Unit u : punits)
            result += u.getPower();
        return result;
    }
    
    public boolean isContested() {
        if(!isOwned()) {
            return units.size() != 0;
        } else {
            
            for(Unit u: units)
                if(!u.getPlayer().equals(owner))
                    return true;
        }
        
        return false;
    }
    public boolean isOwned() { return this.owner != null; }
    public void setOwner(Player owner) {this.owner = owner; } 
    public Player getOwner() { return this.owner;}
    
    public void setRadius(float radius) {
        if(radius >= PLANET_MIN_RADIUS)
            this.radius = radius;
    }
    public float getRadius() {return radius;}
    
    public void setName(String name) {this.name = name;}
    public String getName() {return name;}
    
    public void setPosition(Vec2f position) {this.position = position;}
    public Vec2f getPosition() {return position;}
    
    @Override
    public boolean equals(Object other) {
        if(other == null)
            return false;
        
        Planet x = (Planet) other;
        return x.getName().equals(name);
    }
    
    @Override
    public String toString() {
        return getName();
    }
}
