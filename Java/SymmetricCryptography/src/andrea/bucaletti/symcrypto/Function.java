/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

/**
 *
 * @author Andrea
 */
public abstract class Function {
    
    protected InputStream in;

    public Function(InputStream in) { this.in = in; }
    
    protected byte[] read(int max) {
        return read(max, false);
    }
    
    protected byte[] read(int max, boolean zeroPadding) {
        int num = 0;
        byte[] data = new byte[max];

        try {
           num = in.read(data, 0, max);
           if(num <= 0)
               return null;
        }
        catch(IOException ex) {
            return null;
        }
        
        if(zeroPadding && num < max) {
            return data;
        } else {
            byte[] result = new byte[num];
            
            for(int i = 0; i < num; i++)
                result[i] = data[i];
            
            return result;
        }
    } 
    
    public abstract byte[] execute();
}
