/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.test;

import java.io.IOException;
import progettosicurezza.lib.rsa.RSA;
import progettosicurezza.lib.rsa.RSAKeyPair;
import progettosicurezza.libutil.Utility;

/**
 *
 * @author Andrea
 */
public class TestRSA {
    public static void main(String[] args) throws IOException {
        String x = "Ciao stronzi!";
        RSAKeyPair keys = RSA.GenerateKeys(9733, 10177);
        byte m[] = x.getBytes();
        byte c[] = RSA.ArrayCipher8(keys.PublicKey, m);
        byte m2[] = RSA.ArrayDecipher8(keys.SecretKey, c);
       
        
        printArray("m: ", m);
        printArray("c: ", c);
        printArray("m2: ", m2);        
    }
    
    public static void printArray(String title, long[] x) {
        System.out.print(title);
        for(int i = 0; i < x.length; i++)
            System.out.print(x[i] + " ");
        System.out.print("\n");        
    }
    
    public static void printArray(String title, byte[] x) {
        System.out.print(title);
        for(int i = 0; i < x.length; i++)
            System.out.print(x[i] + " ");
        System.out.print("\n");
    }    
}
