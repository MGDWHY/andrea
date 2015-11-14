/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.cards;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class Card implements Serializable {

    /* Ranks are from 0 to 3
     *  Suits are from 0 to 12, with 0 = ACE, 10 = JACK, 11 = QUEEN, 12 = KING
     */
    private static final String[] STR_SUITS = {
        "Hearts", "Diamonds", "Clubs", "Spades"
    };
    private static final String[] STR_RANKS = {
        "Ace", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "Jack", "Queen", "King"
    };
    public static final int HEARTS = 0;
    public static final int DIAMONDS = 1;
    public static final int CLUBS = 2;
    public static final int SPADES = 3;
    private int suit;
    private int rank;

    public static Card FromString(String str) {
        int suit = Integer.parseInt(str.charAt(0) + "");
        int rank = Integer.parseInt(str.charAt(1) + "");
        return new Card(suit, rank);
    }

    public Card(int suit, int rank) throws IllegalArgumentException {

        if (suit >= 0 && suit <= 3 && rank >= 0 && rank <= 12) {
            this.rank = rank;
            this.suit = suit;
        } else {
            throw new IllegalArgumentException("Invalid suit or rank: " + suit + " " + rank);
        }
    }

    public String getStringRepresentation() {
        return STR_RANKS[rank] + " of " + STR_SUITS[suit];
    }

    public int getSuit() {
        return suit;
    }

    public int getRank() {
        return rank;
    }
}
