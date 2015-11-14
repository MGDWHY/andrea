/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.test;

import java.awt.image.BufferedImage;
import java.io.File;
import projectconquest.data.Image;
import projectconquest.data.java.JavaImage;

/**
 *
 * @author Andrea
 */
public class ProvaImg {
    public static void main(String[] args) throws Exception {
        
        Image img = JavaImage.loadFromFile(new File("prova.jpg"));
        BufferedImage img2 = new JavaImage(img);
    }
}
