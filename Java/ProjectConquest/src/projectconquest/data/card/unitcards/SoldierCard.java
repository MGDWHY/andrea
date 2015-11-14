/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.card.unitcards;

import projectconquest.data.card.Card;
import projectconquest.data.card.UnitCard;

/**
 *
 * @author Andrea
 */
public class SoldierCard extends UnitCard {
    
    public SoldierCard(int id) {
        super(Card.TID_SOLDIER, id);
    }

    @Override
    public int getUnitTemplateID() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public int getResourceCost() {
        return 2;
    }
    
}
