/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.java;

import projectconquest.data.TileSet;

/**
 *
 * @author Andrea
 */
public class JavaTileSet {
    
    protected int tileSize;
    
    protected JavaImage[] tiles;
    
    public JavaTileSet(TileSet src) {
        
        tileSize = src.getTileSize();
        
        tiles = new JavaImage[src.getTilesCount()];
        
        tiles[0] = null;
        
        for(int i = 1; i < tiles.length; i++)
            tiles[i] = new JavaImage(src.getTile(i));
        
    }
    
    public JavaImage getTile(int index) {
        return tiles[index];
    }
    
    
    public int getTilesCount() {
        return tiles.length;
    }    
    
    public int getTileSize() {
        return tileSize;
    }
}
