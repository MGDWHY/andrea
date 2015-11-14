/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.libutil;

/**
 *
 * @author Andrea
 */
public class Utility {
    public static byte[] LongToBytes(long l) {
        byte[] data = new byte[8];
        for(int i = 0; i < 8; i++)
            data[7-i] = (byte)(l >>> i * 8);
        
        return data;
    }
    
    public static long BytesToLong(byte[] b) {
        long l = 0;
        for(int i = 0; i < 8; i++) {
            l = l << 8;
            l |= (long)b[i] & 0xFF;
        }
        return l;
    }
    
}
