/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto.tests;

import andrea.bucaletti.symcrypto.cfb.CFBCipher;
import andrea.bucaletti.symcrypto.cfb.CFBDecipher;
import andrea.bucaletti.symcrypto.counter.CounterFunction;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *
 * @author Andrea
 */
public class Test5 {
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
        
        long key;
        String text;
        
        print("Enter key: ");
        key = Long.parseLong(readLine());
        

        print("Enter text: ");
        text = readLine();
        
        
        CounterFunction cipher = new CounterFunction(new ByteArrayInputStream(text.getBytes()));
        cipher.setKey(key);
        byte[] data = cipher.execute();
        
        println("Encrypted text: " + new String(data));
        
        CounterFunction decipher = new CounterFunction(new ByteArrayInputStream(data));
        decipher.setKey(key);
        data = decipher.execute();
        
        println("Decrypted text: " + new String(data));
        
    }        
}
