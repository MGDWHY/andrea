/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.gamelogic.cards.commmon;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.cards.FreeCard;

/**
 *
 * @author Andrea
 * Draw 2 cards
 */
public class LuckOfTheStarsCard extends FreeCard {

    public static final String NAME = "Luck of the Stars";
    public static final int RESOURCE_COST = 0;   
    
    public static final int CARDS_GIVEN = 2;
    
    private static final long serialVersionUID = 1935130629818727690L;
    
    public LuckOfTheStarsCard() { super(Constants.IDC_LUCK_OF_THE_STARS, RESOURCE_COST, Constants.CK_COMMON);}
    
    @Override
    public void play(GameMap gameMap) {
        player.addCardsToDraw(CARDS_GIVEN);
        while(player.drawCard());
        played();
    }

    @Override
    public String getName() {
        return NAME;
    }
}
