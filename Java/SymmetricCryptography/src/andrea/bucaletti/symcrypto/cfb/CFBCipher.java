/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.cfb;

import andrea.bucaletti.symcrypto.KeyFunction;
import com.sun.xml.internal.ws.message.ByteArrayAttachment;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Random;

/**
 *
 * @author Andrea
 */
public class CFBCipher extends KeyFunction<Integer> {
    
    private byte[] iv;
    
    public CFBCipher(InputStream in) { super(in); }
    
    public void setIV(String iv) {
        if(iv.length() == 8)
            this.iv = iv.getBytes();
        else
            throw new IllegalArgumentException("Invalid IV: " + iv);
    }
    
    public byte[] getIV() {
        return iv;
    }

    @Override
    public boolean acceptKey(Integer key) {
        return true;
    }

    @Override
    public byte[] execute() {
        Random rnd = new Random(getKey());
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        byte[] register = Arrays.copyOf(iv, 8);
        byte[] dataIn;
        byte[] dataOut;
        
        while((dataIn = read(4)) != null) {
            int len = dataIn.length;
            dataOut = new byte[len];
            
            for(int i = 0; i < len; i++) { // encryption
                byte r = (byte)(-128 + rnd.nextInt(256));
                dataOut[i] = (byte)(dataIn[i] ^ register[i] ^ r);
            }
            
            int count = 0;
            
            while(count < len) { // shift the register
               for(int i = 1; i < 8; i++)
                   register[i-1] = register[i];
               
               register[7] = dataOut[count++];
            }
            
            out.write(dataOut, 0, len);
        }
        
        return out.toByteArray();
    }
    
}
