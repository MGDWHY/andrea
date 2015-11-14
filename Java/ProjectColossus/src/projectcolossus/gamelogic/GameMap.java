/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;

/**
 *
 * @author Andrea
 */
public class GameMap implements Serializable {
    private static final long serialVersionUID = -5837706952183036512L;
    
    private int playerNumber;
    
    private ArrayList<Planet> planets;
    private ArrayList<Route> routes;
    
    private Planet startingPlanets[];
    
    public static GameMap loadFromFile(File file) throws IOException, ClassNotFoundException {
        return load(new FileInputStream(file));
    }
    
    public static GameMap load(InputStream is) throws IOException, ClassNotFoundException {
        ObjectInputStream in = new ObjectInputStream(is);
        GameMap result = (GameMap) in.readObject();
        in.close();
        return result;       
    }
    
    public GameMap(int playerNumber) {
        this.playerNumber = playerNumber;
        this.startingPlanets = new Planet[playerNumber];
        this.planets = new ArrayList<Planet>();
        this.routes = new ArrayList<Route>();
    }
    
    public Vec2f getSize() {
        float mx = 0, my = 0;
        for(Planet p : planets) {
            mx = p.getPosition().getX() > mx ? p.getPosition().getX() : mx;
            my = p.getPosition().getY() > my ? p.getPosition().getY() : my;
        }
        
        return new Vec2f(mx, my);
    }
   
    public void setPlayerStartingPlanet(int player, Planet p) {this.startingPlanets[player] = p;}
    public void setPlayerStartingPlanet(Player player, Planet p) { this.startingPlanets[player.index] = p; }
    public Planet getPlayerStartingPlanet(int playerIndex) {return startingPlanets[playerIndex];}
    public Planet getPlayerStartingPlanet(Player player) {return startingPlanets[player.index];}

    public boolean addRoute(Route r) {
        if(!routes.contains(r)) {
            routes.add(r);
            return true;
        } else
            return false;
    }
    
    public boolean addPlanet(Planet p) {
        if(!planets.contains(p)) {
            planets.add(p);
            return true;
        } else 
            return false;
    }
    
    public boolean removePlanet(Planet p) {
        
        deleteRoutesToPlanet(p);

        return planets.remove(p);            
    }
    
    public void deleteRoutesToPlanet(Planet p) {
        Iterator<Route> it = routes.iterator();
        while(it.hasNext()) {
            if(it.next().containsPlanet(p))
                it.remove();
        }        
    }
    

    
    public int getPlayerNumber() {
        return playerNumber;
    }
    
    public ArrayList<Planet> getConnectedPlanets(Planet p) {
        ArrayList<Planet> result = new ArrayList<Planet>();
        
        for(Route r : routes)
            if(r.containsPlanet(p))
                result.add(r.getOtherPlanet(p));
        
        return result;
        
    }
    
    public ArrayList<Route> getConnectedRoutes(Planet p) {
        ArrayList<Route> result = new ArrayList<Route>();
        
        for(Route r : routes)
            if(r.containsPlanet(p))
                result.add(r);
        
        return result;
    }    
    
    public ArrayList<Planet> getPlanets() {
        return planets;
    }
    
    public ArrayList<Route> getRoutes() {
        return routes;
    }
    

}
