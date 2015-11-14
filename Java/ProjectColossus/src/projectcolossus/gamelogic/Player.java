/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;


import java.io.Serializable;
import java.util.ArrayList;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.util.Util;
/**
 *
 * @author Andrea
 */
public abstract class Player implements Serializable {

    
    public static final int[] DEFAULT_COLORS = {
        0xffF7CB19,
        0xffE368C2,
        0xff00ff00,
        0xff00ffff
    };
    private static final long serialVersionUID = -7476476109706717160L;
    
    protected int index;
    
    protected int color;
    
    protected float[] floatColor;
    
    protected int resources;
    
    protected int drawableCards;
    
    protected String name;
    
    protected ArrayList<Unit> units;
    
    protected Deck deck;
    protected ArrayList<Card> hand;
    
    public Player(int index, String name) {
        this.name = name;
        this.index = index;
        this.units = new ArrayList<Unit>();
        this.hand = new ArrayList<Card>();
        this.floatColor = new float[3];
        this.drawableCards = 0;
        
        setColor(DEFAULT_COLORS[index]);
    }
    
    public void setDeck(Deck deck) {
        this.deck = deck;
        deck.shuffle();
    }
    
    public Deck getDeck() {
        return deck;
    }
    
    public int getDrawableCards() {
        return drawableCards;
    }
    
    public void addCardsToDraw(int amount) {
        drawableCards += amount;
    }
    
    public Card hasCard(String name) {
        for(int i = 0; i < hand.size(); i++)
            if(hand.get(i).getName().equals(name))
                return hand.get(i);
        
        return null;
    }
    
    public boolean drawCard() {
        if(drawableCards > 0) {
            Card c = deck.drawCard();
            if(c != null) {
                c.setPlayer(this);
                hand.add(c);
                drawableCards--;
                return true;
            } else
                return false;
        } else
            return false;
    }
    
    public boolean playedCard(Card c) {
        boolean result = hand.remove(c);
        if(result)
            resources -= c.getResourceCost();
        return result;
    }
    
    public int getPlayerIndex() {
        return index;
    }
    
    public boolean addUnit(Unit u) {
        return units.add(u);
    }
    
    public boolean removeUnit(Unit u) {
        return units.remove(u);
    }
    
    public Unit getUnitByID(int id) {
        for(Unit u : units)
            if(u.getID() == id)
                return u;
        
        return null;
    }
    
    public ArrayList<Unit> getUnits() {
        return units;
    }
    
    public ArrayList<Card> getHand() {
        return hand;
    }
    
    public void setColor(int color) {
        this.color = color;
        this.floatColor = Util.intColorToFloat(color);
    }
    
    public int getColor() {
        return color;
    }
    
    public float[] getFloatColor() {
        return floatColor;
    }
    
    public int getIndex() {
        return index;
    }
    
    public String getName() {
        return name;
    }
    
    public int getResources() {
        return resources;
    }
    
    public void setResources(int r) {
        this.resources = r;
    }
    
    public void addResources(int n) {
        this.resources += n;
    }
    
    public void removeResources(int n) {
        this.resources -= n;
    }
    
    public abstract void play();
    
    @Override
    public boolean equals(Object x) {
        if(x == null)
            return false;
        Player other = (Player)x;
        return index == other.index;
    }
}
