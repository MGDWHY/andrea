/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.util.LockableArrayList;

/**
 *
 * @author Andrea
 * 
 * Fasi del gioco:
 * - begin turn
 *      - draw cards
 *      - planet buffs onBeginTurn()
 *      - update buffs with UPDATE_ON_BEGIN
 * - play
 *      - resolve battles and conquest
 * - end turn
 *      - apply buffs onEndTurn()
 *      - update buffs with UPDATE_ON_END
 */
public class GameData implements Serializable {
    private static final long serialVersionUID = -8122060322212844405L;
    
    protected int currentPlayer;
    
    protected GameMap gameMap;
    
    protected Player[] players;
    
    protected int getResourcesAmount(Player player, Planet planet, ArrayList<Planet> discardList) {
        int result = 0;
        int index = player.index;
        
        if(discardList == null)
            discardList = new ArrayList<Planet>();
        
        if(discardList.contains(planet))
            return 0;
        
        if(!planet.isOwned())
            return 0;
        else {
            if(!planet.getOwner().equals(player))
                return 0;
            else {
                ArrayList<Route> connectedRoutes = gameMap.getConnectedRoutes(planet);
                
                discardList.add(planet);
                
                for(Route r : connectedRoutes)
                    result += getResourcesAmount(player, r.getOtherPlanet(planet), discardList);
                
                result += planet.getResourcesPerTurn();
                
                return result;                
            }
        }
        
    }
   
    public GameData(GameMap gameMap) {
        this.currentPlayer = 0;
        this.gameMap = gameMap;
        this.players = new Player[gameMap.getPlayerNumber()];
        for(int i = 0; i < gameMap.getPlanets().size(); i++) {
            Planet planet = gameMap.getPlanets().get(i);
            ConquestData data = new ConquestData(planet, players);
            planet.setConquestData(data);
        }
    }
    
    public Unit getUnitByID(int id) {
        for(Player p : players)
            for(Unit u : p.getUnits())
                if(u.getID() == id)
                    return u;
        
        return null;
    }
    
    public Planet getPlanetByID(int id) {
        for(Planet p : gameMap.getPlanets())
            if(p.id == id)
                return p;
        
        return null;
    }
    
    public Planet getPlanetByName(String name) {
        
        for(Planet p : gameMap.getPlanets())
            if(p.getName().equals(name))
                return p;
        
        return null; 
    }
    
    /*
     * A player can see friendly planets and their neighbours,
     * and planets where they have units and their neighbours
     */
    public ArrayList<Planet> getVisiblePlanets(Player player) {
        LockableArrayList<Planet> visiblePlanets = new LockableArrayList<Planet>();
        
        for(Planet p : gameMap.getPlanets())
            if(p.isOwned() && p.getOwner().equals(player))
                visiblePlanets.add(p);
        
        for(Unit u : player.getUnits())
            visiblePlanets.addUnique(u.getPlanet());
        
        visiblePlanets.lock();
        
        for(Planet p : visiblePlanets) {
            ArrayList<Planet> neighbours = gameMap.getConnectedPlanets(p);
            for(Planet n : neighbours)
                visiblePlanets.addUnique(n);
        }
             
        visiblePlanets.unlock();
        
        return visiblePlanets;
    }    
    
    public ArrayList<Planet> getVisiblePlanets(int playerIndex) {
        return getVisiblePlanets(players[playerIndex]);
    }
    
    public GameMap getGameMap() {
        return this.gameMap;
    }
    
    public Player[] getPlayers () {
        return players;
    }
    
    public List<Player> getPlayersOnPlanet(int planetID) {
        ArrayList<Player> result = new ArrayList<Player>();
        Planet planet = getPlanetByID(planetID);
        
        for(Unit u : planet.getUnits())
            if(!result.contains(u.getPlayer()))
                result.add(u.getPlayer());
        
        return result;
    }
    
    public void setPlayer(Player player) {
        this.players[player.index] = player;
        gameMap.getPlayerStartingPlanet(player).setOwner(player);
        player.addCardsToDraw(4);
    }
    
    public Player getCurrentPlayer() {
        return players[currentPlayer];
    }
    
    public Player getPlayer(int index) {
        return players[index];
    }
    
    public int getConquestCounter(Planet planet, Player player) {
        return planet.getConquestData().getConquestCounter(player.index);
    }
    
    public void beginTurn() {
        
        Player cp = players[currentPlayer];
        
        cp.addCardsToDraw(1);
        ArrayList<Unit> playerUnits = cp.getUnits();
        
        cp.setResources(getResourcesAmount(cp, gameMap.getPlayerStartingPlanet(cp.index), null));
        
        for(Unit u: playerUnits)
            u.resetMovement();
        
        for(Planet p: gameMap.getPlanets()) {
            p.getBuffs().lock();
            
            for(PlanetBuff b : p.getBuffs()) {
                b.onTurnBegin(cp);
                b.beginTurn(cp);
            }
            p.getBuffs().unlock();
        }
    }
    
    public void endTurn() {
        
        Player cp = players[currentPlayer];

        for(Planet p: gameMap.getPlanets()) {
            
            p.getBuffs().lock();
            
            for(PlanetBuff b : p.getBuffs()) {
                b.onTurnEnd(cp);
                b.endTurn(cp);
            }
            
            p.getBuffs().unlock();
        }
        
        currentPlayer = (currentPlayer + 1) % gameMap.getPlayerNumber();
    }
    
    public void playTurn() {
        ArrayList<Planet> planets = gameMap.getPlanets();
      
        for(Planet p : planets)
            p.getConquestData().updateConquest();
    }
    
    public static class ConquestData implements Serializable {
        
        public static final int DEFAULT_MAX_VALUE = 100;
        
        protected Planet planet;
        
        protected Player[] players;
        protected int numPlayers;
        protected int maxValue;
        protected int conquestCounters[];
        
        public ConquestData(Planet planet, Player[] players) {
            this.numPlayers = players.length;
            this.conquestCounters = new int[numPlayers];
            this.players = players;
            this.planet = planet;
            this.maxValue = DEFAULT_MAX_VALUE;
        }
        
        public void setMaxValue(int value) { 
            this.maxValue = value;
            updateCounters();
        }
        public int getMaxValue() { 
            return this.maxValue;
        }
        public void addToMaxValue(int amount) {
            maxValue += amount;
            updateCounters();
        }
        
        public int getConquestCounter(int player) {
            return conquestCounters[player];
        }
        
        public int getConquestCounter(Player player) {
            return conquestCounters[player.index];
        }
        
        public void updateConquest() {
            
            int powers[] = new int[numPlayers];
            int playersOnPlanet = 0;
            int max = -1, winnerIndex = -1;
            
            if(planet.getUnits().size() == 0)
                return; // no battles on this planet
            
            for(int i = 0; i < numPlayers; i++) {
                powers[i] = planet.getPlayerPower(players[i]);
                
                if(powers[i] > 0)
                    playersOnPlanet++;
                
                if(powers[i] > max) {
                    max = powers[i];
                    winnerIndex = i;
                }
            }          

            if(playersOnPlanet == 1) { // just one player, conquer the planet
                for(int i = 0; i < numPlayers; i++)
                    if(i != winnerIndex)
                        decreaseCounter(i, powers[winnerIndex] - powers[i]);
                
                if(increaseCounter(winnerIndex, powers[winnerIndex])) {
                    planet.setOwner(players[winnerIndex]);
                }                 
                
            } else { // battles between players;
                
                Unit.PowerComparator pc = new Unit.PowerComparator();
                
                for(int i = 0; i < numPlayers; i++) {
                    for(int j = 0; j < numPlayers; j++) {
                        if(i != j) {                 
                            if(powers[i] > powers[j] && powers[j] > 0) {
                                List<Unit> loserUnits = planet.getPlayerUnits(players[j]);
                                Collections.sort(loserUnits, pc);
                                loserUnits.get(0).kill();
                            }   
                        }
                    }
                }
            }
        }
        
        protected boolean increaseCounter(int player, int amount) {
            conquestCounters[player] += amount;
            
            if(conquestCounters[player] >= maxValue) {
                conquestCounters[player] = maxValue;
                return true;
            } else
                return false;
        }
        
        protected boolean decreaseCounter(int player, int amount) {
            conquestCounters[player] -= amount;
            if(conquestCounters[player] <= 0) {
                conquestCounters[player] = 0;
                return true;
            } else 
                return false;
        }
        
        protected void updateCounters() {
            for(int i = 0; i < conquestCounters.length; i++)
                if(conquestCounters[i] > maxValue)
                    conquestCounters[i] = maxValue;
        }
    }
}
