/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.wep;

import andrea.bucaletti.symcrypto.KeyFunction;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Random;

/**
 *
 * @author Andrea
 * Cifrario WEP - Vulnerabile ad attacchi di tipo statistico data la limitata dimensione 
 * della chiave. 
 * Un punto debole è dato dal fatto che l'IV viene trasmesso in chiaro. Quindi un utente
 * delle rete (che quindi conosce la chiave), potrebbe intercettare e decifrare
 * le informazioni dirette ad altri tramite cercando di "indovinare l'IV. Inoltre
 * poichè la chiave funge da seed per il PRNG, questa deve essere di dimensioni limitate
 * (al massimo long - 8 bytes - se si usa la classe Random di Java)
 */
public class WEPCipher extends KeyFunction<Integer> {
    
    private int iv;
    
    public WEPCipher(InputStream in) {
        super(in);
    }
    
    public void setIV(int iv) {
        this.iv = iv;
    }

    @Override
    public boolean acceptKey(Integer key) {
        return true;
    }

    @Override
    public byte[] execute() {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        DataOutputStream outData = new DataOutputStream(out);
        
        Random rnd = new Random(getKey() + iv);
        
        byte[] dataIn;
        byte[] dataOut;
        
        try {
            outData.writeInt(iv);
        }
        catch(IOException ex) {}
        
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
