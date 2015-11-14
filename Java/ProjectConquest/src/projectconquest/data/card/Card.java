/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.card;

/**
 *
 * @author Andrea
 */
public abstract class Card {
    
    /* Card types */
    public static final int TYPE_UNIT = 1;
    
    /* Cards template ids */
    
    /* Common bio units */
    public static final int TID_SOLDIER = 1;
    
    
    
    protected int templateID;
    
    protected int id;
    
    public Card(int templateID, int id) {
        this.templateID = templateID;
        this.id = id;
    }
    
    public int getID() { return id; }
    
    public int getTemplateID() { return templateID; }
    
    public abstract int getType();
    
    public abstract int getResourceCost();
    
}
