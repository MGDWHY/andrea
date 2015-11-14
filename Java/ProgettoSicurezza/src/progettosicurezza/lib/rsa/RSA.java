/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.rsa;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.util.Random;
import progettosicurezza.libutil.Utility;

/**
 *
 * @author Andrea
 */
public class RSA {
    
    
    public static RSAKeyPair GenerateKeys(int P, int Q) {
        long N = P * Q, E = 0, D = 0;
        long Phi = (P - 1) * (Q - 1);
        Random random = new Random(System.currentTimeMillis());
        
        // Seleziona E e D
        boolean finished = false;
        while(!finished) {
            E = (long) (random.nextDouble() * (Phi - 1));
            long[] mcd = MCDe(E, Phi);
            if(E > 1 && Phi % E != 0 && mcd[0] == 1) {
                D = mcd[1];
                
                while(D < 0)
                    D += Phi;
                
                if(E != D)
                    finished = true;
            }
        }       
       
        RSAKey pKey = new RSAKey(N, E);
        RSAKey sKey = new RSAKey(N, D);
        
        return new RSAKeyPair(pKey, sKey);       
    }
    

    
    // cifratura e firma
    // c = m^E mod N
    public static byte[] Cipher8(RSAKey key, byte m) {
        BigInteger bM = BigInteger.valueOf(m & 0xFF);
              
        return Utility.LongToBytes(bM.modPow(key.bExp, key.bN).longValue());
    }

    public static byte[] ArrayCipher8(RSAKey key, byte[] m) {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        try {
            for(int i = 0; i < m.length; i++)
                out.write(Cipher8(key, m[i]));
            }
        catch(IOException ex) {}
        return out.toByteArray();
    }    
    
    // decifrazione e verfica
    // m = c^D mod N
    public static byte Decipher8(RSAKey key, byte[] c) {
        BigInteger bC = BigInteger.valueOf(Utility.BytesToLong(c));
        
        return (byte)(bC.modPow(key.bExp, key.bN).intValue());
    }
    
    public static byte[] ArrayDecipher8(RSAKey key, byte[] c) {
       ByteArrayOutputStream out = new ByteArrayOutputStream();
       
       for(int i = 0; i < c.length / 8; i++) {
           byte data[] = new byte[8];
           for(int j = 0; j < 8; j++)
               data[j] = c[i * 8 + j];
           
            out.write(Decipher8(key, data));
       }
        
       return out.toByteArray();
    }
    
    
    public static long[] MCDe(long a, long b) {
        long x1 = 0, x2 = 1, y1 = 1, y2 = 0;
        long x, y, q, r;
        
        while(b > 0) {
            q = a / b;
            r = a - q*b;
            x = x2 - q * x1; y = y2 - q * y1;
            a = b; b = r;
            x2 = x1; x1 = x;
            y2 = y1; y1 = y;
        }
        
        long[] result = {a, x2, y2};
        
        return result;
        
    }

}
