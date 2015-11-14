/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.player;

import projectcolossus.gamelogic.Player;

/**
 *
 * @author Andrea
 */
public class HumanPlayer extends Player {
    
    private static final long serialVersionUID = -3441656734383812860L;
    
    public HumanPlayer(int index, String name) {
        super(index, name);
    }

    @Override
    public void play() {
        while(drawCard());
    }
    
}
