/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.prototype;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.util.List;
import javax.swing.JPanel;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.Route;
import projectcolossus.gamelogic.Vec2f;

/**
 *
 * @author Andrea
 */
public class GamePanel extends JPanel {
    
    private Planet highLight;

    private GameMap gameMap;
    
    private GameData gameData;
    
    private Player player;

    public GamePanel() {
        super();
    }
    
    public void setGameData(GameData data) {
        this.gameMap = data.getGameMap();
        this.gameData = data;
    }
    
    public void setHighlighPlanet(Planet p) {
        this.highLight = p;
    }
    
    public void update(Player player) {
        this.player = player;
        this.revalidate();
        this.repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;       
        
        int fontHeight = g2.getFontMetrics().getHeight();

        g2.setBackground(Color.black);
        
        g2.clearRect(0, 0, getWidth(), getHeight());
        
        if(gameMap == null || player == null)
            return;
        
        g2.setPaint(Color.WHITE);        
        
        for(Route r : gameMap.getRoutes()) {
            Vec2f pos0 = r.getPlanet(0).getPosition();
            Vec2f pos1 = r.getPlanet(1).getPosition();
            
            g2.drawLine((int)pos0.getX(), (int)pos0.getY(), (int)pos1.getX(), (int)pos1.getY());
            
        }        
        
        for(Planet planet: gameData.getVisiblePlanets(player)) {
            int stringWidth = 0, y = 0;
            
            Vec2f position = planet.getPosition();
            
            if(planet.getOwner() != null)
                g2.setPaint(new Color(planet.getOwner().getColor()));
            else
                g2.setPaint(Color.white);
            // draw planet
            Ellipse2D shape = new Ellipse2D.Float(
                position.getX() - planet.getRadius(),
                position.getY() - planet.getRadius(),
                planet.getRadius() * 2,
                planet.getRadius() * 2
            );
            
            g2.fill(shape);
            
            stringWidth = g2.getFontMetrics().stringWidth(planet.getName());
            
            g2.drawString(planet.getName(), position.getX() - stringWidth / 2, position.getY() - planet.getRadius() - fontHeight);
            
            // draw units
            g2.translate(position.getX(), position.getY());
            y = (int)-planet.getRadius();
            for(Player p : gameData.getPlayers()) {
                int units = planet.getPlayerUnits(p).size();
                g2.setPaint(new Color(p.getColor()));
                for(int x = 0; x < units; x++) {
                    g2.fillRect((int)planet.getRadius() + 10 * (x + 1), y, 5, 5);
                }
                y += 10;
            }
            g2.translate(-position.getX(), -position.getY());
            
            // draw conquest points
            y = 0;
            
            for(Player player: gameData.getPlayers()) {
                g2.setPaint(new Color(player.getColor()));
                String str = gameData.getConquestCounter(planet, player) + "/" + planet.getConquestData().getMaxValue();
                stringWidth = g2.getFontMetrics().stringWidth(str);
                g2.drawString(str, position.getX() - stringWidth / 2, position.getY() + planet.getRadius() + 10 * (1+y++));
            }
            
            // draw highlight circle
            if(planet.equals(highLight)) {
                
                shape = new Ellipse2D.Float(
                    position.getX() - planet.getRadius() - 10,
                    position.getY() - planet.getRadius() - 10,
                    planet.getRadius() * 2 + 20,
                    planet.getRadius() * 2 + 20
                );
                
                g2.setPaint(new Color(player.getColor()));
                
                g2.draw(shape);
                
            }
            
            
        }

    } 
}
