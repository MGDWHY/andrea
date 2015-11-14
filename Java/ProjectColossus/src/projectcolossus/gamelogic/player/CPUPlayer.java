/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.player;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.commmon.LuckOfTheStarsCard;

/**
 *
 * @author Andrea
 */
public class CPUPlayer extends Player {
    
    private static final String FREE_CARDS[] = {
        LuckOfTheStarsCard.NAME
    };
    
    private static final long serialVersionUID = 719068917953508622L;
    
    protected GameData gameData;

    public CPUPlayer(int index, String name, GameData gameData) {
        super(index, name);
        this.gameData = gameData;
    }
    
    @Override
    public void play() {
        while(drawCard());
        
        /*
        for(String c : FREE_CARDS) {
            Card card = hasCard(c);
            if(card != null)
                card.play();
        }*/
    }
    
}
