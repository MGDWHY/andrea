/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.mapeditor;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;
import projectconquest.data.TileLayer;
import projectconquest.data.java.JavaTileSet;
import projectconquest.data.map.Location;
import projectconquest.data.map.Map;
import projectconquest.data.map.Route;

/**
 *
 * @author Andrea
 */
public class MapPanel extends JPanel {
    
    public static final float LOCATION_RADIUS = 8.0f;
    public static final float TILE_SIZE = 64; // pixel
    
    private Location highLight;

    private Map map;
    
    private JavaTileSet tileSet;
    
    private boolean showGrid;

    public MapPanel() {
        super();
        showGrid = true;
    }
    
    public void setMap(Map map) {
        this.map = map;
        
        if(map.getTileSet() != null) 
            this.tileSet = new JavaTileSet(map.getTileSet());
        
        setPreferredSize(new Dimension(map.getWidth() * (int)TILE_SIZE, map.getHeight() * (int)TILE_SIZE));
        setSize(map.getWidth() * (int)TILE_SIZE, map.getHeight() * (int)TILE_SIZE);
        
        invalidate();
        repaint();
        
    }
    
    public void setHightlightLocation(Location l) {
        this.highLight = l;
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;       
        
        int fontHeight = g2.getFontMetrics().getHeight();
        
        g2.clearRect(0, 0, getWidth(), getHeight());
        
        if(map == null)
            return;

        
        g2.setPaint(Color.BLACK);
        
        for(int i = 0; i < map.getTileLayers().size(); i++) {
            TileLayer layer = map.getTileLayers().get(i);

            for(int y = 0; y < map.getHeight(); y++)
                for(int x = 0; x < map.getWidth(); x++) {

                    BufferedImage tile = tileSet.getTile(layer.getTileIndexAt(x, y));

                    if(tile != null) {
                        g2.drawImage(tile,
                                (int)(x * TILE_SIZE),
                                (int)(y * TILE_SIZE),
                                (int)((x + 1) * TILE_SIZE),
                                (int)((y + 1) * TILE_SIZE),
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
        
        
        if(showGrid) {
            
            g2.setPaint(Color.GRAY);
            
            for(int x = 0; x < map.getWidth() + 1; x++) {
                g2.drawLine(x * (int) TILE_SIZE, 0, x * (int) TILE_SIZE, map.getHeight() * (int)TILE_SIZE);
                
            }
            
            for(int y = 0; y < map.getHeight() + 1; y++) {
                g2.drawLine(0, y * (int) TILE_SIZE, map.getWidth() * (int) TILE_SIZE, y * (int) TILE_SIZE);
                
            }            
            
        }        
            
       
        
        for(Route r : map.getRoutes()) {
            Location l0 = r.getLocation(0);
            Location l1 = r.getLocation(1);
            
            g2.drawLine((int)(l0.getX() * TILE_SIZE), (int)(l0.getY() * TILE_SIZE), 
                    (int)(l1.getX() * TILE_SIZE), (int)(l1.getY() * TILE_SIZE));
            
        }
        
        for(Location l : map.getLocations()) {
            if(highLight != null && highLight.equals(l))
                g2.setPaint(Color.RED);
            else
                g2.setPaint(Color.BLACK);


            Ellipse2D shape = new Ellipse2D.Float(
                l.getX() * TILE_SIZE - LOCATION_RADIUS,
                l.getY() * TILE_SIZE - LOCATION_RADIUS,
                LOCATION_RADIUS * 2,
                LOCATION_RADIUS  * 2
            );              

            g2.fill(shape);
        }
      

    } 
}
