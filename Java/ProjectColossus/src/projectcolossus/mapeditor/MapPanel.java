/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.mapeditor;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.util.List;
import javax.swing.JPanel;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Route;
import projectcolossus.gamelogic.Vec2f;

/**
 *
 * @author Andrea
 */
public class MapPanel extends JPanel {
    
    private Planet highLight;

    private GameMap gameMap;

    public MapPanel() {
        super();
    }
    
    public void setGameMap(GameMap map) {
        this.gameMap = map;
    }
    
    public void setHighlighPlanet(Planet p) {
        this.highLight = p;
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;       
        
        int fontHeight = g2.getFontMetrics().getHeight();

        g2.setBackground(Color.black);
        
        g2.clearRect(0, 0, getWidth(), getHeight());
        
        if(gameMap == null)
            return;
        
        g2.setPaint(Color.WHITE);        
        
        for(Route r : gameMap.getRoutes()) {
            Vec2f pos0 = r.getPlanet(0).getPosition();
            Vec2f pos1 = r.getPlanet(1).getPosition();
            
            g2.drawLine((int)pos0.getX(), (int)pos0.getY(), (int)pos1.getX(), (int)pos1.getY());
            
        }        
        
        for(Planet p: gameMap.getPlanets()) {
            Vec2f position = p.getPosition();
            
            if(highLight != null && highLight.equals(p))
                g2.setPaint(Color.RED);
            else
                g2.setPaint(Color.WHITE);
            
            Ellipse2D shape = new Ellipse2D.Float(
                position.getX() - p.getRadius(),
                position.getY() - p.getRadius(),
                p.getRadius() * 2,
                p.getRadius() * 2
            );
            
            g2.fill(shape);
            
            int stringWidth = g2.getFontMetrics().stringWidth(p.getName());
            
            g2.drawString(p.getName(), position.getX() - stringWidth / 2, position.getY() - p.getRadius() - fontHeight);
            
            
        }

    } 
}
