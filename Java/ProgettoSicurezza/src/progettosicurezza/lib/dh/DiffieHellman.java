/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.dh;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Random;
import progettosicurezza.lib.pki.PKI;

/**
 *
 * @author Andrea
 */
public class DiffieHellman {
    
    private static final int DEFAULT_G = 9733;
    private static final int DEFAULT_P = 10177;
    
    private long p, g;
    
    private long y;
    private long x;
    private long r1, r2;
    
    private Random random;
    
    public DiffieHellman() {
        this(DEFAULT_G, DEFAULT_P);
    }
    
    public DiffieHellman(long g, long p) {
        this.p = p;
        this.g = g;
        
        random = new Random(System.currentTimeMillis());
        
        x = random.nextLong();
        r1 = random.nextLong();
        
        y = BigInteger.valueOf(g).modPow(BigInteger.valueOf(x), BigInteger.valueOf(p)).longValue();
        
    }
    
    public long GetY() { // parametro pubblico
        return y;
    }
    
    public long GetX() { // parametro privato
        return x;
    }
    
    public long GetFirstRandom() {
        return r1;
    }
    
    public void SetSecondRandom(long r2) {
        this.r2 = r2;
    }
    
    public byte[] GenerateKey(long yOther) throws IOException, NoSuchAlgorithmException {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        MessageDigest hf = MessageDigest.getInstance(PKI.HASH_ALGORITHM);
        BigInteger val = BigInteger.valueOf(yOther).modPow(BigInteger.valueOf(x), BigInteger.valueOf(p));
        
        os.write(val.toByteArray());
        os.write(BigInteger.valueOf(r1 ^ r2).toByteArray());
        
        byte[] data = os.toByteArray();
        
        os.close();
        
        return hf.digest(data);
    }
}
