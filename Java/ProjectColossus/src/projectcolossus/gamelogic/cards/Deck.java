/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 *
 * @author Andrea
 */
public class Deck implements Serializable {

    public static final int MAX_CARDS = 30;
    private static final long serialVersionUID = 1580575834851284066L;
    
    protected ArrayList<Card> cards;
    
    public Deck() {
        cards = new ArrayList<Card>();
    }
    
    public String[] getCardNames() {
        String[] c = new String[cards.size()];
        for(int i = 0; i < cards.size(); i++)
            c[i] = cards.get(i).getName();
        
        return c;
    }
    
    public ArrayList<Card> getCards() {
        return cards;
    }
    
    public boolean addCard(Card c) {
        if(cards.size() < MAX_CARDS)
            return cards.add(c);
        else
            return false;
    }
    
    public void shuffle() {
        Collections.shuffle(cards);
    }
    
    public Card drawCard() {
        if(cards.size() > 0) {
            Card c = cards.get(0);
            cards.remove(0);
            return c;
        } else
            return null;
    }
    
    public static void write(OutputStream out, Deck deck) throws IOException {
        DataOutputStream dos = new DataOutputStream(out);
        
        dos.writeInt(deck.getCards().size());
 
        for(Card c : deck.cards)
            dos.writeUTF(c.getName());
    }
    
    public static Deck read(InputStream in) throws IOException {
        Deck result = new Deck();
        DataInputStream dis = new DataInputStream(in);
        
        int size = dis.readInt();

        while(size-- > 0) {
            String c = dis.readUTF();
            result.addCard(Card.forName(c));
        }
        
        return result;
        
    }
}
