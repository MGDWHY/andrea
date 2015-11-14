/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.test;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;
import projectconquest.data.TileLayer;
import projectconquest.data.java.JavaTileSet;
import projectconquest.data.map.Map;

/**
 *
 * @author Andrea
 */
public class GamePanel extends JPanel implements MouseListener, MouseMotionListener {
    
    /* Screen resolution */
    private ClientPrototype.Resolution resolution;
    
    /* Transform properties */
    private float tx = 0.0f, ty = 0.0f;
    
    private float scaling = 4.0f;
    private float heightScaling = 0.0f;
    
    /* Mouse events */
    private boolean isDragging = false;
    private float prevX, prevY;
    
    /* Game events */
    private boolean isGameRunning = true;
    
    /* Graphics lock */
    private Object gLock = new Object();
    
    /* Graphics */
    private Map map;
    private JavaTileSet tileSet;    
    private AffineTransform scaleTransform, translateTransform;
    
    public GamePanel() {
        super();
        scaleTransform = new AffineTransform();
        translateTransform = new AffineTransform();
        addMouseListener(this);
        addMouseMotionListener(this);
    }
    
    public void runGame() {
        isGameRunning = true;
        invalidate();
        repaint();
    }
    
    public void waitForPlayers() {
        isGameRunning = false;
        invalidate();
        repaint();
    }
    
    public void setResolution(ClientPrototype.Resolution r) {
        
        synchronized(gLock) {
            this.resolution = r;

            this.setPreferredSize(new Dimension(r.getWidth(), r.getHeight()));
            this.setSize(r.getWidth(), r.getHeight());
            
            heightScaling = scaling * ((float)r.getHeight() / r.getWidth());
            
            scaleTransform.setToScale(r.getWidth() / scaling, r.getHeight() / heightScaling);
        }
        invalidate();
        repaint();
        
    }
    
    public void setMap(Map map) {
        
        synchronized(gLock) {
            this.map = map;

            if(map.getTileSet() != null) 
                this.tileSet = new JavaTileSet(map.getTileSet());
        }
        
        invalidate();
        repaint();        
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        
        
        
        if(map == null)
            return;
           
        synchronized(gLock) {
            AffineTransform t = new AffineTransform();
            t.concatenate(scaleTransform);
            t.concatenate(translateTransform);
   

            g2.setTransform(t);      
   
            for(int i = 0; i < map.getTileLayers().size(); i++) {
                TileLayer layer = map.getTileLayers().get(i);

                for(int y = 0; y < map.getHeight(); y++) {
                    for(int x = 0; x < map.getWidth(); x++) {

                        BufferedImage tile = tileSet.getTile(layer.getTileIndexAt(x, y));

                        if(tile != null) {
                            g2.drawImage(tile,
                                    x,
                                    y,
                                    x + 1,
                                    y + 1,
                                    0,
                                    0,
                                    tileSet.getTileSize(),
                                    tileSet.getTileSize(),
                                    null,
                                    null
                            );
                        }
                    }  
                }
            }
            
            if(!isGameRunning) {
                
                g2.setTransform(new AffineTransform());
                
                Color color = new Color(0, 0, 0, .75f); //Red 
                g2.setPaint(color);
                g2.fillRect(0, 0, resolution.getWidth(), resolution.getHeight());             
                
                g2.setPaint(Color.RED);
                g2.drawString("Waiting for players", 20, 20);
            }
            
            
        }
        
    }

    @Override
    public void mouseClicked(MouseEvent me) {
        
    }

    @Override
    public void mousePressed(MouseEvent me) {
        isDragging = true;
        prevX = scaling * (float)me.getX() / resolution.getWidth();
        prevY = heightScaling * (float)me.getY() / resolution.getHeight();
    }

    @Override
    public void mouseReleased(MouseEvent me) {
        isDragging = false;
    }

    @Override
    public void mouseEntered(MouseEvent me) { }

    @Override
    public void mouseExited(MouseEvent me) {}

    @Override
    public void mouseDragged(MouseEvent me) {
        if(isDragging) {
            synchronized(gLock) {
                float x = scaling * (float)me.getX() / resolution.getWidth();
                float y = heightScaling * (float)me.getY() / resolution.getHeight();
                tx = tx + (x - prevX);
                ty = ty + (y - prevY);
                
                if(tx > 0)
                    tx = 0;
                else if(tx < -(map.getWidth() - scaling))
                    tx = -(map.getWidth() - scaling);
                
                if(ty > 0)
                    ty = 0;
                else if (ty < -(map.getHeight() - heightScaling))
                    ty = -(map.getHeight() - heightScaling);
                
                translateTransform.setToTranslation(tx, ty);     

                prevX = x;
                prevY = y;
            }
            
            invalidate();
            repaint();
        }
    }    

    @Override
    public void mouseMoved(MouseEvent me) {}
  

    
}
