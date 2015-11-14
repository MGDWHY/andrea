/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.tests;

import andrea.bucaletti.symcrypto.caesarcipher.CaesarFunction;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *
 * @author Andrea
 */
public class Test1 {
    
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
        
        print("Enter key: ");
        
        int key = Byte.parseByte(readLine());
        
        print("Enter text: ");
        
        String text = readLine();
        
        
        CaesarFunction cipher = new CaesarFunction(new ByteArrayInputStream(text.getBytes()));
        cipher.setKey(key);
        byte[] data = cipher.execute();
        
        println("Encrypted text: " + new String(data));
        
        CaesarFunction decipher = new CaesarFunction(new ByteArrayInputStream(data));
        decipher.setKey(-key);
        data = decipher.execute();
        
        println("Decrypted text: " + new String(data));
        
    }
    
}
