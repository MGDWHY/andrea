/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards;

import java.io.Serializable;
import java.util.HashMap;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.commmon.LuckOfTheStarsCard;
import projectcolossus.gamelogic.cards.commmon.NuclearMissile;
import projectcolossus.gamelogic.cards.commmon.OrbitalDefenceCard;
import projectcolossus.gamelogic.cards.commmon.PlanetaryCageCard;
import projectcolossus.gamelogic.cards.commmon.PlanetaryExplotationCard;
import projectcolossus.gamelogic.cards.commmon.TacticalBombingCard;
import projectcolossus.gamelogic.cards.heirs.AegisCard;
import projectcolossus.gamelogic.cards.heirs.GhostCard;
import projectcolossus.gamelogic.cards.heirs.HunterCard;
import projectcolossus.gamelogic.cards.heirs.MechaSquadCard;
import projectcolossus.gamelogic.cards.heirs.RaiderCard;
import projectcolossus.gamelogic.cards.heirs.ScoutCard;
import projectcolossus.gamelogic.units.Unit;

/**
 *
 * @author Andrea
 */
public abstract class Card implements Serializable {

    
    private static final HashMap<String, Class> cards;
    private static final long serialVersionUID = -184824069139179711L;
    
    protected int kind;
    protected int id;
    protected int resourceCost;
    protected Player player;
    
    static {
        cards = new HashMap<String, Class>();
        
        // Heirs cards
        cards.put(HunterCard.NAME, HunterCard.class);
        cards.put(ScoutCard.NAME, ScoutCard.class);
        cards.put(RaiderCard.NAME, RaiderCard.class);
        cards.put(GhostCard.NAME, GhostCard.class);
        cards.put(MechaSquadCard.NAME, MechaSquadCard.class);
        cards.put(AegisCard.NAME, AegisCard.class);
        
        // Common
        cards.put(LuckOfTheStarsCard.NAME, LuckOfTheStarsCard.class);
        cards.put(PlanetaryExplotationCard.NAME, PlanetaryExplotationCard.class);
        cards.put(OrbitalDefenceCard.NAME, OrbitalDefenceCard.class);
        cards.put(TacticalBombingCard.NAME, TacticalBombingCard.class);
        cards.put(PlanetaryCageCard.NAME, PlanetaryCageCard.class);
        cards.put(NuclearMissile.NAME, NuclearMissile.class);
    }
    
    public static String[] getAvailableCards() {
        String[] cardNames = new String[cards.size()];
        return cards.keySet().toArray(cardNames);
    }
    
    public static Card forName(String name) {
   
        try {
            Class c = cards.get(name);

            return (Card)c.newInstance();
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
        
        return null;
    }
    
    public Card(int id, int resourceCost, int kind) {
        this.id = id;
        this.resourceCost = resourceCost;
        this.kind = kind;
    }
    
    public final void setPlayer(Player player) {
        this.player = player;
    }
    
    public final Player getPlayer() {
        return player;
    }
    
    public final int getResourceCost() {
        return resourceCost;
    }
    
    public final int getID() {
        return id;
    }    
    
    public final int getKind() {
        return kind;
    }
    
    public final boolean isPlayable() {
        return getResourceCost() <= this.player.getResources();
    }
    
    @Override
    public String toString() {
        return getName();
    }
    /**
     * Subclasses MUST call this method when this card is played. It will remove the card
     * from the player's hand
     */
    protected final void played() {
        this.player.playedCard(this);
    }
    
    public abstract boolean canPlayRegardless();
    public abstract boolean canPlayOnPlanet(GameMap gameMap, Planet planet);
    public abstract boolean canPlayOnUnit(Unit unit);
    
    public abstract void playOnPlanet(GameMap gameMap, Planet planet);
    public abstract void play(GameMap gameMap);
    
    public abstract String getName();
}
