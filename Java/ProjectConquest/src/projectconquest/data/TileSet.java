/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data;

import java.io.Serializable;
import java.util.HashMap;

/**
 *
 * @author Andrea
 */
public class TileSet implements Serializable {
    private static final long serialVersionUID = 6513917800943278487L;
    
    protected int tileSize;
    
    protected HashMap<Integer, Image> tiles;
    
    public TileSet(int tileSize) throws IllegalArgumentException {
        
        this.tileSize = tileSize;
        this.tiles = new HashMap<>();
        
    }
    
    public void addTiles(Image image, int startIndex) {
        /*if(image.getWidth() % tileSize != 0 || image.getHeight() % tileSize != 0)
            throw new IllegalArgumentException("Image is not splittable in tiles");*/
     
        int tilew = image.width / tileSize;
        int tileh = image.height / tileSize;
        
        for(int j = 0; j < tileh; j++)
            for(int i = 0; i < tilew; i++) {
                tiles.put(startIndex + j * tilew + i, createTile(image, i, j));
            }        
    }
    
    public Image getTile(int index) {
        return tiles.get(index);
    }
    
    public int getTilesCount() {
        return tiles.size();
    }
    
    public int getTileSize() {
        return tileSize;     
    }
    
    protected Image createTile(Image src, int i, int j) {
        Image tile = new Image(tileSize, tileSize);
        
        for(int x = 0; x < tileSize; x++)
            for(int y = 0; y < tileSize; y++) {
                tile.setPixel(x, y, src.getPixel(i * tileSize + x, j * tileSize + y));
            }
        
        return tile;
    }
}
