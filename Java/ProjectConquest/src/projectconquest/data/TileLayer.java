/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class TileLayer implements Serializable {
    
    private static final long serialVersionUID = 6949588920858021906L;
    
    private int tileSize;
    
    private int width, height;
    
    private int[] tiles;
    
    public TileLayer(int width, int height, int tileSize) {
        this.width = width;
        this.height = height;
        this.tileSize = tileSize;
        this.tiles = new int[width * height];
    }
    
    public void setTiles(int[] tiles) throws IllegalArgumentException {
        if(tiles.length != width * height)
            throw new IllegalArgumentException("Wrong number of tiles: expected " + width * height + ", found: " + tiles.length);
        
        this.tiles = tiles;
    }
    
    public int getTileIndexAt(int x, int y) {
        return tiles[y * width + x];
    }

    
    public int getWidth() { return width; }
    
    public int getHeight() { return height; }
}
