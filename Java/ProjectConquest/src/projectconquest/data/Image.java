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
public class Image implements Serializable {
    private static final long serialVersionUID = -7849851490630002911L;
    
    protected int width, height;
    
    protected int[] pixels;
    
    public Image(int width, int height) {
        this.width = width;
        this.height = height;
        this.pixels = new int[width * height];
    }
    
    public void setPixel(int x, int y, int argb) {
        this.pixels[y * width + x] = argb;
    }
    
    public int getPixel(int x, int y) {
        return this.pixels[y * width + x];
    }
    
    public int getWidth() { return width; }
    
    public int getHeight() { return height; }
    
}
