/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.tests;

import andrea.bucaletti.symcrypto.wep.WEPCipher;
import andrea.bucaletti.symcrypto.wep.WEPDecipher;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *
 * @author Andrea
 */
public class Test2 {
    public static String readLine() throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        return reader.readLine();
    }
    
    public static void print(String s) {
        System.out.print(s);
    }
    
    public static void println(String s) {
        System.out.println(s);
    }
    
    public static void main(String[] args) throws IOException {
        
        int key = 2384732;
        
        print("Enter IV: ");
        
        int iv = Integer.parseInt(readLine());
        
        print("Enter text: ");
        
        String text = readLine();
        
        
        WEPCipher cipher = new WEPCipher(new ByteArrayInputStream(text.getBytes()));
        cipher.setKey(key);
        cipher.setIV(iv);
        byte[] data = cipher.execute();
        
        println("Encrypted text: " + new String(data));
        
        WEPDecipher decipher = new WEPDecipher(new ByteArrayInputStream(data));
        decipher.setKey(key);
        data = decipher.execute();
        
        println("Decrypted text: " + new String(data));
        
    }    
}
