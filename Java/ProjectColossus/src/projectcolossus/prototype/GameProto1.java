/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.prototype;

import java.io.FileInputStream;
import java.net.InetAddress;
import java.util.ArrayList;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JOptionPane;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.PlanetBuff;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.client.Client;
import projectcolossus.gamelogic.server.ServerCommand;
import projectcolossus.gamelogic.units.Unit;

/**
 *
 * @author Andrea
 */
public class GameProto1 extends javax.swing.JFrame implements GamePrototype, Client.Listener {
    
    public static final String SERVER_IP = "127.0.0.1";
    public static String MAP_FILE = "data/map.pcm";
    public static String DECK_FILE = "data/deck.pcd";
    public static String STATE_FILE = "data/state";
    
    /**
     * Creates new form GameProto1
     */
    
    private Planet selectedPlanet;
    
    private GameMap gameMap;
    private GameData gameData;
    
    private Player player;
    private Client client;
    
    public GameProto1() {
        initComponents();
        
        try {
        
            Deck deck = Deck.read(new FileInputStream(DECK_FILE));

            client = new Client();

            client.addListener(this);
                 
            ServerCommand.MMGameServerData data = client.findMatch(InetAddress.getByName(SERVER_IP), MMServerRun.SERVER_PORT);
            
            System.out.println(data.ServerAddress);
            
            client.connect(InetAddress.getByName(SERVER_IP), data.ServerPort, "Player", deck);
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }
    
    private void updateGameData(GameData gameData) {

        this.gameData = gameData;
        this.gameMap = gameData.getGameMap();        
       
        gamePanel.setGameData(gameData);
        
        cardsPanel.setClient(client);
        cardsPanel.setGameData(gameData);
        cardsPanel.setGamePrototype(this);    
        
        Planet[] planets = new Planet[1];
        planets = gameMap.getPlanets().toArray(planets);
        DefaultComboBoxModel<Planet> planetOptions = new DefaultComboBoxModel<Planet>(planets);
        
        cmoMoveUnit.setModel(new DefaultComboBoxModel<Unit>());
        
        cmoPlanet.setModel(planetOptions);

        updateAll();             
        
        cardsPanel.update(player);
        gamePanel.update(player);    
    }
    
    protected Planet planetAt(Vec2f position) {
        for(Planet p : gameMap.getPlanets()) {
            float distance = position.substract(p.getPosition()).length();
            if(distance < p.getRadius()) {
                return p;
            }
        }
        return null;
    }    
    
    public void updateAll() {
        lblTurn.setText("Turn:" + gameData.getCurrentPlayer().getIndex() + " Resources:" + player.getResources());
        btnDrawCard.setText("Draw Card (" + gameData.getCurrentPlayer().getDrawableCards() + ")");
        if(selectedPlanet != null) {
            
            Unit[] alliedUnits = new Unit[1];
            alliedUnits = selectedPlanet.getPlayerUnits(player).toArray(alliedUnits);
            DefaultComboBoxModel<Unit> moveUnitOptions = new DefaultComboBoxModel<Unit>(alliedUnits);
            cmoMoveUnit.setModel(moveUnitOptions);
            
            String planetInfo = "<html>";
                    
            planetInfo += "<strong>Selected planet: " + selectedPlanet.getName() + "</strong><br>Buffs:<br>";
            
            for(PlanetBuff buff : selectedPlanet.getBuffs())
                planetInfo += buff.getName() + "(" + buff.getApplier().getName() + ", " + buff.getTurnsLeft() + ")<br>";
            
            planetInfo += "</html>";
            
            lblSelectedPlanet.setText(planetInfo);
            
            String forces = "<html>";
            for(int i = 0; i < gameData.getGameMap().getPlayerNumber(); i++) {
                Player p = gameData.getPlayer(i);
                ArrayList<Unit> units = selectedPlanet.getPlayerUnits(p);
                forces += "<html><strong>" + p.getName() + ": Power " + selectedPlanet.getPlayerPower(p)
                        + " Conquest " + gameData.getConquestCounter(selectedPlanet, p) + "/" + selectedPlanet.getConquestData().getMaxValue() + "</strong><ul>";
                for(Unit u : units)
                    forces += "<li>" + u.getName() + "</li>";
                forces += "</ul>";
            }
            forces += "</html>";
            lblForces.setText(forces);     
        }
        
        cardsPanel.update(player);
        gamePanel.update(player);        
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        gamePanel = new projectcolossus.prototype.GamePanel();
        pnlProperties = new javax.swing.JPanel();
        btnEndTurn = new javax.swing.JButton();
        btnDrawCard = new javax.swing.JButton();
        lblTurn = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        javax.swing.JPanel pnlPlanetInfo = new javax.swing.JPanel();
        lblSelectedPlanet = new javax.swing.JLabel();
        lblForces = new javax.swing.JLabel();
        cmoMoveUnit = new javax.swing.JComboBox();
        javax.swing.JLabel jLable1 = new javax.swing.JLabel();
        javax.swing.JLabel jLabel1 = new javax.swing.JLabel();
        cmoPlanet = new javax.swing.JComboBox();
        btnMove = new javax.swing.JButton();
        cardsPanel = new projectcolossus.prototype.CardsPanel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        gamePanel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseReleased(java.awt.event.MouseEvent evt) {
                gamePanelMouseReleased(evt);
            }
        });

        javax.swing.GroupLayout gamePanelLayout = new javax.swing.GroupLayout(gamePanel);
        gamePanel.setLayout(gamePanelLayout);
        gamePanelLayout.setHorizontalGroup(
            gamePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 860, Short.MAX_VALUE)
        );
        gamePanelLayout.setVerticalGroup(
            gamePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 487, Short.MAX_VALUE)
        );

        pnlProperties.setBackground(new java.awt.Color(0, 255, 0));

        btnEndTurn.setText("End turn");
        btnEndTurn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnEndTurnActionPerformed(evt);
            }
        });

        btnDrawCard.setText("Draw card");
        btnDrawCard.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnDrawCardActionPerformed(evt);
            }
        });

        lblTurn.setText("Turn");

        pnlPlanetInfo.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        lblSelectedPlanet.setFont(new java.awt.Font("Tahoma", 1, 11)); // NOI18N
        lblSelectedPlanet.setText("Selected Planet");

        lblForces.setText("Forces");

        javax.swing.GroupLayout pnlPlanetInfoLayout = new javax.swing.GroupLayout(pnlPlanetInfo);
        pnlPlanetInfo.setLayout(pnlPlanetInfoLayout);
        pnlPlanetInfoLayout.setHorizontalGroup(
            pnlPlanetInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlPlanetInfoLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnlPlanetInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pnlPlanetInfoLayout.createSequentialGroup()
                        .addComponent(lblSelectedPlanet)
                        .addGap(0, 212, Short.MAX_VALUE))
                    .addComponent(lblForces, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        pnlPlanetInfoLayout.setVerticalGroup(
            pnlPlanetInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlPlanetInfoLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(lblSelectedPlanet)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lblForces)
                .addContainerGap(378, Short.MAX_VALUE))
        );

        jScrollPane1.setViewportView(pnlPlanetInfo);

        jLable1.setText("Move");

        jLabel1.setText("to planet");

        btnMove.setText("Go");
        btnMove.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnMoveActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pnlPropertiesLayout = new javax.swing.GroupLayout(pnlProperties);
        pnlProperties.setLayout(pnlPropertiesLayout);
        pnlPropertiesLayout.setHorizontalGroup(
            pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlPropertiesLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                    .addGroup(pnlPropertiesLayout.createSequentialGroup()
                        .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(lblTurn)
                            .addGroup(pnlPropertiesLayout.createSequentialGroup()
                                .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, pnlPropertiesLayout.createSequentialGroup()
                                        .addComponent(jLabel1)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(cmoPlanet, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, pnlPropertiesLayout.createSequentialGroup()
                                        .addComponent(jLable1)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(cmoMoveUnit, javax.swing.GroupLayout.PREFERRED_SIZE, 171, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(btnMove, javax.swing.GroupLayout.PREFERRED_SIZE, 57, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(pnlPropertiesLayout.createSequentialGroup()
                                .addComponent(btnEndTurn, javax.swing.GroupLayout.PREFERRED_SIZE, 117, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(btnDrawCard, javax.swing.GroupLayout.PREFERRED_SIZE, 121, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(0, 49, Short.MAX_VALUE)))
                .addContainerGap())
        );
        pnlPropertiesLayout.setVerticalGroup(
            pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnlPropertiesLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(lblTurn)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnEndTurn)
                    .addComponent(btnDrawCard))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 404, Short.MAX_VALUE)
                .addGap(18, 18, 18)
                .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(cmoMoveUnit, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLable1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnlPropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(cmoPlanet, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(btnMove))
                .addGap(62, 62, 62))
        );

        cardsPanel.setBackground(new java.awt.Color(153, 255, 255));

        javax.swing.GroupLayout cardsPanelLayout = new javax.swing.GroupLayout(cardsPanel);
        cardsPanel.setLayout(cardsPanelLayout);
        cardsPanelLayout.setHorizontalGroup(
            cardsPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        cardsPanelLayout.setVerticalGroup(
            cardsPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 100, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(gamePanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(cardsPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(pnlProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(pnlProperties, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(gamePanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cardsPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnDrawCardActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnDrawCardActionPerformed
        // TODO add your handling code here:
        gameData.getCurrentPlayer().drawCard();
        updateAll();
        
    }//GEN-LAST:event_btnDrawCardActionPerformed

    private void gamePanelMouseReleased(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_gamePanelMouseReleased
        // TODO add your handling code here:
   
        Planet clickedPlanet = planetAt(new Vec2f(evt.getX(), evt.getY()));
        
        if(clickedPlanet != null) {
            if(clickedPlanet.equals(selectedPlanet))
                selectedPlanet = null;
            else
                selectedPlanet = clickedPlanet;
        } else {
            selectedPlanet = null;
        }
        
        gamePanel.setHighlighPlanet(selectedPlanet);
        cardsPanel.setSelectedPlanet(selectedPlanet); 
        
        updateAll();
        
    }//GEN-LAST:event_gamePanelMouseReleased

    private void btnEndTurnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnEndTurnActionPerformed
        // TODO add your handling code here:
        client.endTurn();
    }//GEN-LAST:event_btnEndTurnActionPerformed

    private void btnMoveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnMoveActionPerformed
        // TODO add your handling code here:
        Unit u = (Unit)cmoMoveUnit.getSelectedItem();
        Planet p = (Planet)cmoPlanet.getSelectedItem();
                
        if(u != null && p != null) {
            client.moveUnit(u.getID(), u.getPlanet().getID(), p.getID());
        }  
    }//GEN-LAST:event_btnMoveActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(GameProto1.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(GameProto1.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(GameProto1.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(GameProto1.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */

         new GameProto1().setVisible(true);
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnDrawCard;
    private javax.swing.JButton btnEndTurn;
    private javax.swing.JButton btnMove;
    private projectcolossus.prototype.CardsPanel cardsPanel;
    private javax.swing.JComboBox cmoMoveUnit;
    private javax.swing.JComboBox cmoPlanet;
    private projectcolossus.prototype.GamePanel gamePanel;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblForces;
    private javax.swing.JLabel lblSelectedPlanet;
    private javax.swing.JLabel lblTurn;
    private javax.swing.JPanel pnlProperties;
    // End of variables declaration//GEN-END:variables

    @Override
    public void onUpdate(Client client, Player player, GameData gameData) {
        if(selectedPlanet != null) {
            selectedPlanet = gameData.getPlanetByName(selectedPlanet.getName());
            gamePanel.setHighlighPlanet(selectedPlanet);
            cardsPanel.setSelectedPlanet(selectedPlanet);             
        }
        this.player = player;
        updateGameData(gameData);    
    }

    @Override
    public void onBeginTurn(Client client, int currentPlayer) {
        this.player = gameData.getPlayer(currentPlayer);
        JOptionPane.showMessageDialog(this, "It's your turn");
    }

    @Override
    public void onBadRequest(Client client, int refCommand, int errorCode) {
        JOptionPane.showMessageDialog(this, "Bad request: " + refCommand + " Error code: " + errorCode);
    }

    @Override
    public void onUnitMoved(Client client, int unitID, int fromPlanetID, int toPlanetID) {}

}
