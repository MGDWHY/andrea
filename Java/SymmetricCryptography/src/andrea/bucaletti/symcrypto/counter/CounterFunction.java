/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.counter;

import andrea.bucaletti.symcrypto.KeyFunction;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 *
 * @author Andrea
 */
public class CounterFunction extends KeyFunction<Long> {
    
    private long counter;
    
    public CounterFunction(InputStream is) { super(is); }
    
    @Override
    public boolean acceptKey(Long key) {
        return true;
    }

    @Override
    public byte[] execute() {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        byte[] dataIn, dataOut, counterBytes;
        counter = getKey();
       
        
        while((dataIn = read(8)) != null) {
            dataOut = new byte[dataIn.length];
            counterBytes = getCounterBytes();
            
            for(int i = 0; i < dataOut.length; i++)
                dataOut[i] = (byte)(dataIn[i] ^ counterBytes[i]);
            
            out.write(dataOut, 0, dataOut.length);
            
            counter++;
        }
        
        return out.toByteArray();
    }
    
    protected byte[] getCounterBytes() {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(os);
        try {
            dos.writeLong(counter);
        }
        catch(IOException ex) {}
        return os.toByteArray();
    }
    
}
