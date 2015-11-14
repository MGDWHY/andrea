/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.pki;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import progettosicurezza.lib.rsa.RSA;
import progettosicurezza.lib.rsa.RSAKey;

/**
 *
 * @author Andrea
 */
public class Certificate {
    public final String UserID;
    public final RSAKey PublicKey;
    public final int ListeningPort;
    public byte[] Signature;
    
    public Certificate(String userID, RSAKey publicKey, int listeningPort) {
        this.UserID = userID;
        this.PublicKey = publicKey;
        this.ListeningPort = listeningPort;
    }

    public Certificate(String userID, RSAKey publicKey, int listeningPort, byte[] signature) {
        this.UserID = userID;
        this.PublicKey = publicKey;
        this.ListeningPort = listeningPort;
        this.Signature = signature;
    }    
    
    public void Sign(RSAKey secretKey) {
        String str = UserID + PublicKey.toString();
        
        try {
            MessageDigest md5 = MessageDigest.getInstance(PKI.HASH_ALGORITHM);
            byte[] hash = md5.digest(str.getBytes());
            Signature = RSA.ArrayCipher8(secretKey, hash);
        }
        catch(NoSuchAlgorithmException ex) {
            ex.printStackTrace();
        }
    }
    
    public boolean Verify() {
        return Verify(PublicKey);
    }
    
    public boolean Verify(RSAKey publicKey) {
        
        try {
            MessageDigest md5 = MessageDigest.getInstance(PKI.HASH_ALGORITHM);
            String str = UserID + PublicKey.toString();
            byte[] h1 = md5.digest(str.getBytes());
            byte[] h2 = RSA.ArrayDecipher8(publicKey, Signature);
            
            for(int i = 0; i < h1.length; i++) {
                if(h1[i] != h2[i])
                    return false;
            }
            
            return true;
        }
        catch(NoSuchAlgorithmException ex) {
            ex.printStackTrace();
        }
        
        return false;
    }
    
    public static void write(DataOutputStream dos, Certificate request) throws IOException {
        dos.writeUTF(request.UserID);
        RSAKey.write(dos, request.PublicKey);
        dos.writeInt(request.ListeningPort);
        dos.writeInt(request.Signature.length);
        for(int i = 0; i < request.Signature.length; i++)
            dos.writeByte(request.Signature[i]);
    }
    
    public static Certificate read(DataInputStream dis) throws IOException {
        String userID = dis.readUTF();
        RSAKey publicKey = RSAKey.read(dis);
        int listeningPort = dis.readInt();
        int popLen = dis.readInt();
        byte[] signature = new byte[popLen];
        for(int i = 0; i < popLen; i++)
            signature[i] = dis.readByte();
        
        return new Certificate(userID, publicKey, listeningPort, signature);

    }
    
    public String toString() {
        return UserID;
    }
}
