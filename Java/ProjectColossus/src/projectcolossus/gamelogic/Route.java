/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Route implements Serializable {
    private static final long serialVersionUID = -1736603586683764521L;
   
    
    private Planet[] planets;
    
    public Route(Planet p0, Planet p1) {
        planets = new Planet[2];
        planets[0] = p0;
        planets[1] = p1;
    }
    
    public Planet getOtherPlanet(Planet x) {
        if(x.equals(planets[0]))
            return planets[1];
        else if(x.equals(planets[1]))
            return planets[0];
        else
            return null;
    }
    
    public void setPlanet(int index, Planet p) {
        planets[index] = p;
    }
    
    public Planet getPlanet(int index) {
        return planets[index];
    } 
    
    public boolean containsPlanet(Planet p) {
        return planets[0].equals(p) || planets[1].equals(p);
    }
    
    public String toString() {
        return "(" + planets[0].getName() + ", " + planets[1].getName() + ")";
    }
    
    public boolean equals(Object other) {
        Route r = (Route) other;
        
        return (planets[0].equals(r.planets[0]) && planets[1].equals(r.planets[1]))
                || (planets[1].equals(r.planets[0]) && planets[0].equals(r.planets[1]));
        
    }

}
