/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.card;

/**
 *
 * @author Andrea
 */
public abstract class UnitCard extends Card {

    public UnitCard(int templateId, int id) {
        super(templateId, id);
    }
    
    @Override
    public int getType() {
        return Card.TYPE_UNIT;
    }
    
    public abstract int getUnitTemplateID();
    
}
