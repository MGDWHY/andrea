/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.cards;

/**
 *
 * @author Andrea
 */
import java.util.ArrayList;
import java.util.Collections;

public class CardsDeck {

    private ArrayList<Card> cards;

    private CardsDeck() {
    }

    public static CardsDeck createCardsDeck52() {
        CardsDeck deck = new CardsDeck();

        deck.cards = new ArrayList<Card>();

        for (int s = 0; s <= 3; s++) {
            for (int r = 0; r <= 12; r++) {
                deck.cards.add(new Card(s, r));
            }
        }

        return deck;
    }

    public static CardsDeck createCardsDeck40() {
        CardsDeck deck = new CardsDeck();

        deck.cards = new ArrayList<Card>();

        for (int s = 0; s <= 3; s++) {

            for (int r = 0; r <= 6; r++) {
                deck.cards.add(new Card(s, r));
            }

            for (int r = 10; r <= 12; r++) {
                deck.cards.add(new Card(s, r));
            }
        }
        return deck;
    }

    public void shuffle() {
        Collections.shuffle(cards);
    }

    public int getCardsCount() {
        return cards.size();
    }

    public boolean isEmpty() {
        return cards.isEmpty();
    }

    public Card drawCard() {
        if (!isEmpty()) {
            Card result = cards.get(0);
            cards.remove(0);
            return result;
        } else {
            return null;
        }
    }
}
