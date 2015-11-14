/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.ai;

import java.util.ArrayList;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;

/**
 *
 * @author Andrea
 * Find a route to a planet if exists, using a breath first alogorithm
 */
public class RouteFinder {
    
    private GameMap gameMap;
    private Planet origin, destination;
    
    private int maxSteps;
    
    public RouteFinder(GameMap data) {
        this.gameMap = data;
    }
    
    public ArrayList<State> findRoutes(Planet origin, Planet destination, int maxSteps) {
        this.origin = origin;
        this.destination = destination;
        this.maxSteps = maxSteps;
        
        
        ArrayList<State> explored = new ArrayList<State>(); // Explored states
        ArrayList<State> solutions = new ArrayList<State>(); // Solutions found
        ArrayList<State> states = new ArrayList<State>(); // States to explore
        
        ArrayList<Planet> startingPath = new ArrayList<Planet>();
        startingPath.add(origin);
        
        states.add(new State(destination, startingPath, maxSteps));
        
        while(states.size() > 0) { 
            State current = states.get(0);
            states.remove(0);
            
            explored.add(current);
            
            if(current.goalTest()) { // a solution :)
                solutions.add(current);
                continue;
            }
            
            ArrayList<State> children = current.children();
            
            for(State s : children) {
                if(explored.contains(s)) // state explored, cotinue search
                    continue;
                else {
                    states.add(s); // state to explore
                }
            }
        }
        
        return solutions;
        
    }
    
    public class State {
        
        private int stepsLeft;
        private Planet destination;
        private ArrayList<Planet> path;

        public State(Planet destination, ArrayList<Planet> path, int stepsLeft) {
            this.path = path;
            this.destination = destination;
            this.stepsLeft = stepsLeft;       
        }
       
        public ArrayList<State> children() {
            ArrayList<State> children = new ArrayList<State>();
            int newStepsLeft = stepsLeft;
            
            if(newStepsLeft > 0 || newStepsLeft == -1) { // stepLeft == -1 -> no step limit
                
                if(newStepsLeft > 0)
                    newStepsLeft--;
                
                ArrayList<Planet> neighbours = gameMap.getConnectedPlanets(position());            
                for(Planet p : neighbours) {
                    if(!path.contains(p)) { // avoid loops
                        ArrayList<Planet> newPath = (ArrayList<Planet>)path.clone();
                        newPath.add(p);
                        children.add(new State(destination, newPath, newStepsLeft));
                    }
                }
            }
            
            return children;
        }
        
        public ArrayList<Planet> getPath() {
            return path;
        }
    
        
        public boolean goalTest() {
            return destination.equals(position());
        }
        
        public boolean equals(Object x) {
            State s = (State) x;
            
            return path.equals(s.path);
        }
        
        private Planet position() {
            return path.get(path.size() - 1);
        }
        
        public String toString() {
            String result = new String();
            for(Planet p : path)
                result += " -> " + p.getName();
            
            return result;
        }
        
    }
    
}
