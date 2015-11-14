/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.map;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Route implements Serializable {
    
    private static final long serialVersionUID = -1736603586683764521L;

    private Location[] locations;

    public Route(Location p0, Location p1) {
        locations = new Location[2];
        locations[0] = p0;
        locations[1] = p1;
    }

    public Location getOtherLocation(Location x) {
        if(x.equals(locations[0]))
            return locations[1];
        else if(x.equals(locations[1]))
            return locations[0];
        else
            return null;
    }

    public void setLocation(int index, Location p) {
        locations[index] = p;
    }

    public Location getLocation(int index) {
        return locations[index];
    } 

    public boolean containsLocation(Location p) {
        return locations[0].equals(p) || locations[1].equals(p);
    }


    public boolean equals(Object other) {
        Route r = (Route) other;

        return (locations[0].equals(r.locations[0]) && locations[1].equals(r.locations[1]))
                || (locations[1].equals(r.locations[0]) && locations[0].equals(r.locations[1]));

    }    
}
