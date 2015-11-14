/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.card;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import projectconquest.data.card.unitcards.SoldierCard;

/**
 *
 * @author Andrea
 */
public class CardFactory {
    
    /* UNIT TEMPLATES IDs */
    public static final int TID_SOLDIER_CARD = 1;    
    
    private static HashMap<Integer, Class> cards;
    
    static {
        cards = new HashMap<>();
        
        cards.put(TID_SOLDIER_CARD, SoldierCard.class);
    }
    
    public static Card create(int templateID, int id) {
        Class clazz = null;
        
        try {
            clazz = cards.get(templateID);
            
            if(clazz == null)
                throw new ClassNotFoundException("Cant find card template " + templateID);
            
            Constructor constr = clazz.getConstructor(Integer.class);
            return (Card)constr.newInstance(id);
        }
        catch(Exception ex) {
            System.out.println("Exception: " + ex.getClass() + ": " + ex.getMessage());
            return null;
        }
    }
    
}
