/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.wep;

import andrea.bucaletti.symcrypto.KeyFunction;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Random;

/**
 *
 * @author Andrea
 */
public class WEPDecipher extends KeyFunction<Integer> {
    
    public WEPDecipher(InputStream in) {
        super(in);
    }

    @Override
    public boolean acceptKey(Integer key) {
        return true;
    }

    @Override
    public byte[] execute() {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DataInputStream inData = new DataInputStream(in);
        
        byte[] dataIn;
        byte[] dataOut;
        int iv = 0;
        
        try {
            iv = inData.readInt();
        }
        catch(IOException ex) {}
        
        Random rnd = new Random(getKey() + iv);
        
        while((dataIn = read(1024)) != null) {
            dataOut = new byte[dataIn.length];
            
            for(int i = 0; i < dataOut.length; i++) {
                byte b = (byte)(rnd.nextInt(256));
                
                dataOut[i] = (byte)(dataIn[i] ^ b);
                
                
            }
            
            out.write(dataOut, 0, dataOut.length);
        }
        
        return out.toByteArray();
    }     
}
