/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.test;

import java.io.IOException;
import java.security.NoSuchAlgorithmException;
import progettosicurezza.lib.dh.DiffieHellman;
import progettosicurezza.lib.pki.Certificate;
import progettosicurezza.lib.rsa.RSA;
import progettosicurezza.lib.rsa.RSAKeyPair;

/**
 *
 * @author Andrea
 */
public class TestDH {
    public static void main(String[] args) throws IOException, NoSuchAlgorithmException, InterruptedException {
        DiffieHellman dh1 = new DiffieHellman(17, 11);
        Thread.sleep(1000);
        DiffieHellman dh2 = new DiffieHellman(17, 11);
        
        dh1.SetSecondRandom(dh2.GetFirstRandom());
        dh2.SetSecondRandom(dh1.GetFirstRandom());
        
        byte[] key1 = dh1.GenerateKey(dh2.GetY());
        byte[] key2 = dh2.GenerateKey(dh1.GetY());
        
        printArray("k1", key1);
        printArray("k2", key2);
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
