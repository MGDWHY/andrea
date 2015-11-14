/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.card;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;

/**
 *
 * @author Andrea
 */
public class Deck implements Serializable {    
    
    private ArrayList<Card> cards;
    
    public Card draw() {
        Card c = cards.get(0);
        cards.remove(0);
        return c;
    }
    
    public void shuffle() {
        Collections.shuffle(cards);
    }
    
}
