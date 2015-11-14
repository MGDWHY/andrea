/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.util;

/**
 *
 * @author Andrea
 */
public class Util {
    
    public static float[] intColorToFloat(int color) {
        float r = (0x00ff0000 & color) >>> 16;
        float g = (0x0000ff00 & color) >>> 8;
        float b = (0x000000ff & color);
        
        return new float[] {r / 255.0f, g / 255.0f, b /255.0f };
    }
    
    public static int pixelToDP(float dp, float density) {
    	return (int)(dp * density + 0.5f);
    }
    
}
