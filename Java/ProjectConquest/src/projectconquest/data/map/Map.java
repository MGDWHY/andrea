/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.map;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import projectconquest.data.Image;
import projectconquest.data.TileLayer;
import projectconquest.data.TileSet;

/**
 *
 * @author Andrea
 */
public class Map implements Serializable {
    /* Logic */
    
    private static final long serialVersionUID = -1382572665295867059L;
    
    private ArrayList<Location> locations;
    
    private ArrayList<Route> routes;
    
    private int width, height;
    
    private int playerNumber;
    
    private String name;
    
    
    /* Graphics */
    private TileSet tileSet;
    private ArrayList<TileLayer> tileLayers;
    
    public Map(int width, int height) {
        this(width, height, 2);
    }
    
    public Map(int width, int height, int playerNumber) {
        
        this.playerNumber = playerNumber;
        
        this.width = width;
        this.height = height;
        
        this.locations = new ArrayList<>();
        this.routes = new ArrayList<>();
        
        this.name = new String();
        
        this.tileLayers = new ArrayList<>();
    }
    
    public void setTileSet(TileSet ts) { this.tileSet = ts;}
    public TileSet getTileSet() { return tileSet; }
    
    public boolean addTileLayer(TileLayer layer) throws IllegalArgumentException {

        if(layer.getWidth() != width || layer.getHeight() != height)
            throw new IllegalArgumentException("Layer size does not match this map");
        
        return tileLayers.add(layer);
       
    }
    
    public ArrayList<TileLayer> getTileLayers() {
        return tileLayers;
    }
    
    public void setName(String name) { this.name = name; }
    public String getName() { return name; }

    public boolean addRoute(Route r) {
        if(!routes.contains(r)) {
            return routes.add(r);
        } else
            return false;
    }
    
    public boolean addLocation(Location l) {
        if(l.getX() < 0 || l.getX() > width || l.getY() < 0 || l.getY() > height)
            throw new IllegalArgumentException("Location out of bounds");
        return locations.add(l);
    }
    
    public boolean removeLocation(Location l) {
        deleteRoutesToLocation(l);        
        return locations.remove(l);          
    }    
    
    public void deleteRoutesToLocation(Location l) {
        
        if(l == null)
            return;
        
        Iterator<Route> it = routes.iterator();
        while(it.hasNext()) {
            if(it.next().containsLocation(l))
                it.remove();
        }        
    }
    

    
    public int getPlayerNumber() {
        return playerNumber;
    }
    
    public ArrayList<Location> getConnectedLocations(Location l) {
        ArrayList<Location> result = new ArrayList<Location>();
        
        for(Route r : routes)
            if(r.containsLocation(l))
                result.add(r.getOtherLocation(l));
        
        return result;
        
    }
    
    public ArrayList<Route> getConnectedRoutes(Location l) {
        ArrayList<Route> result = new ArrayList<Route>();
        
        for(Route r : routes)
            if(r.containsLocation(l))
                result.add(r);
        
        return result;
    }
    
    public ArrayList<Location> getLocations() {
        return locations;
    }
    
    public ArrayList<Route> getRoutes() {
        return routes;
    }    
    
    
    public int getWidth() { return width; }
    
    public int getHeight() { return height; }

}
