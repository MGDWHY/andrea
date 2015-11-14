/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.cbc;

import andrea.bucaletti.symcrypto.KeyFunction;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Random;

/**
 *
 * @author Andrea
 */
public class CBCCipher extends KeyFunction<Integer> {
    
    private byte[] iv;
    
    public CBCCipher(InputStream in) { super(in); }
    
    public void setIV(String iv) {
        if(iv.length() == 8)
            this.iv = iv.getBytes();
        else
            throw new IllegalArgumentException("Invalid IV: " + iv);
    }
    
    public byte[] getIV() { return iv; }
    
    @Override
    public boolean acceptKey(Integer key) {
        return true;
    }

    @Override
    public byte[] execute() {
        Random rnd = new Random(getKey());
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        byte[] prev = Arrays.copyOf(iv, 8);
        byte[] dataIn;
        byte[] dataOut;
        
        
        while((dataIn = read(8)) != null) {
            
            dataOut = new byte[dataIn.length];
            
            for(int i = 0; i < dataOut.length; i++) {
                byte r = (byte)(-128 + rnd.nextInt(256));
                dataOut[i] = (byte)(dataIn[i] ^ prev[i] ^ r);
                prev[i] = dataOut[i];
            }
            
            out.write(dataOut, 0, dataOut.length);
            
            
        }
        
        return out.toByteArray();
    }
    
}
