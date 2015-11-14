/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.rsa;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.math.BigInteger;

/**
 *
 * @author Andrea
 */
public class RSAKey {
    
    public final long N, Exp;
    
    public final BigInteger bN, bExp; 
    
    public RSAKey(long n, long exp) {
        this.N = n;
        this.Exp = exp;
        this.bN = BigInteger.valueOf(n);
        this.bExp = BigInteger.valueOf(exp);
    }
    
    public String toString() {
        return "(" + N + ", " + Exp + ")";
    }

    public static void write(DataOutputStream dos, RSAKey obj) throws IOException {
        dos.writeLong(obj.Exp);
        dos.writeLong(obj.N);
    }

    public static RSAKey read(DataInputStream dis) throws IOException {
        long exp = dis.readLong();
        long n = dis.readLong();
        return new RSAKey(n, exp);
    }
    
}
