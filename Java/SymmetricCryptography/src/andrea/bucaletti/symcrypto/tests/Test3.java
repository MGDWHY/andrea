/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.tests;

import andrea.bucaletti.symcrypto.cbc.CBCCipher;
import andrea.bucaletti.symcrypto.cbc.CBCDecipher;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *
 * @author Andrea
 */
public class Test3 {
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
        
        int key;
        String iv;
        String text;
        
        print("Enter key: ");
        key = Integer.parseInt(readLine());
        
        print("Enter IV: ");
        iv = readLine();
        
        print("Enter text: ");
        text = readLine();
        
        
        CBCCipher cipher = new CBCCipher(new ByteArrayInputStream(text.getBytes()));
        cipher.setKey(key);
        cipher.setIV(iv);
        byte[] data = cipher.execute();
        
        println("Encrypted text: " + new String(data));
        
        CBCDecipher decipher = new CBCDecipher(new ByteArrayInputStream(data));
        decipher.setKey(key);
        decipher.setIV(iv);
        data = decipher.execute();
        
        println("Decrypted text: " + new String(data));
        
    }        
}
