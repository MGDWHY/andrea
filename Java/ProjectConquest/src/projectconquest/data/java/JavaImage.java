/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.data.java;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import projectconquest.data.Image;

/**
 *
 * @author Andrea
 */
public class JavaImage extends BufferedImage  {
    public JavaImage(Image src) {
        super(src.getWidth(), src.getHeight(), BufferedImage.TYPE_INT_ARGB);
        
        for(int x = 0; x < src.getWidth(); x++)
            for(int y = 0; y < src.getHeight(); y++)
                setRGB(x, y, src.getPixel(x, y));
    }
    
    public static Image loadFromFile(File file) throws IOException {
        BufferedImage bufimg = ImageIO.read(file);
        
        Image img = new Image(bufimg.getWidth(), bufimg.getHeight());
        
        for(int x = 0; x < img.getWidth(); x++)
            for(int y = 0; y < img.getHeight(); y++)
                img.setPixel(x, y, bufimg.getRGB(x, y));
        
        return img;
    }
}
