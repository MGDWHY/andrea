/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.units;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import projectconquest.data.units.common.bio.Soldier;

/**
 *
 * @author Andrea
 */
public class UnitFactory {
    
    /* Unit template ids */
    
    public static final int TID_SOLDIER = 1;
    
    private static HashMap<Integer, Class> units;
    
    static {
        units = new HashMap<>();
        
        units.put(TID_SOLDIER, Soldier.class);
        
    }
    
    public static Unit create(int templateID, int id) {
        Class clazz = null;
        
        try {
            clazz = units.get(templateID);
            
            if(clazz == null)
                throw new ClassNotFoundException("Cant find unit template " + templateID);
            
            Constructor constr = clazz.getConstructor(Integer.class);
            return (Unit)constr.newInstance(id);
        }
        catch(Exception ex) {
            System.out.println("Exception: " + ex.getClass() + ": " + ex.getMessage());
            return null;
        }        
    }
    
}
