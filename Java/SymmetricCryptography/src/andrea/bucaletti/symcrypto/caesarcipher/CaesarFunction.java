/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.caesarcipher;

import andrea.bucaletti.symcrypto.KeyFunction;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.ByteBuffer;
import sun.awt.image.ByteBandedRaster;

/**
 *
 * @author Andrea
 */
public class CaesarFunction extends KeyFunction<Integer> {
    
    public CaesarFunction(InputStream in) { super(in); }

    @Override
    public boolean acceptKey(Integer key) {
        return true;
    }

    @Override
    public byte[] execute() {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        
        byte[] dataIn = null;
        byte[] dataOut = null;
        while((dataIn = read(32)) != null) {
            
            dataOut = new byte[dataIn.length];
            
            for(int i = 0; i < dataOut.length; i++) {
                dataOut[i] = (byte)(dataIn[i] + getKey());
            }
            
            out.write(dataOut, 0, dataOut.length);
        }
        
        return out.toByteArray();
    }
    
}
