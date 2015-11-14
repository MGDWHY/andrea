/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.prototype;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JPanel;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.client.Client;

/**
 *
 * @author Andrea
 */
public class CardsPanel extends JPanel {
   
    private GameData gameData;
    
    private Planet selectedPlanet;
    
    private GamePrototype prototype;
    
    private Client client;
    
    public CardsPanel() {
        super();
    }
    
    public void setClient(Client client) {
        this.client = client;
    }
    
    public void setGamePrototype(GamePrototype prototype) {
        this.prototype = prototype;
    }
    
   public void setSelectedPlanet(Planet p) {
        this.selectedPlanet = p;
    }
    
    public void setGameData(GameData data) {
        this.gameData = data;
    }
    
    public void update(Player player) {
        
        this.invalidate();
        
        setLayout(new FlowLayout(FlowLayout.LEFT));
     
        this.removeAll();
        
        ArrayList<Card> hand = player.getHand();
        
        for(int i = 0; i < hand.size(); i++)
            this.add(new CardButton(this, i, hand.get(i)));
        
        this.revalidate();
        this.repaint();
    }
    
    class CardButton extends JButton implements ActionListener, Runnable{
        
        private int cardIndex;
        private CardsPanel owner;
        private Card card;
        
        public CardButton(CardsPanel owner, int cardIndex, Card card) {
            super(card.getName() + " (" + card.getResourceCost() + ")");
            addActionListener(this);
            
            this.cardIndex = cardIndex;
            this.owner = owner;
            this.card = card;
        }

        @Override
        public void actionPerformed(ActionEvent ae) {
            
            if(!card.isPlayable())
                return;
            
            if(owner.selectedPlanet != null && card.canPlayOnPlanet(gameData.getGameMap(), owner.selectedPlanet)) {
                client.playPlanetCard(cardIndex, owner.selectedPlanet.getID());
            } else if(card.canPlayRegardless()) {
                client.playFreeCard(cardIndex);
            }
            
            
            java.awt.EventQueue.invokeLater(this);
        } 

        @Override
        public void run() {
            prototype.updateAll();
        }
    }
}
