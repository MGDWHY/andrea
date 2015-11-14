/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.rsa;

import java.io.Serializable;

/**
 *
 * @author Andrea
 */
public class RSAKeyPair {
    public final RSAKey PublicKey, SecretKey;
    
    public RSAKeyPair(RSAKey pKey, RSAKey sKey) {
        PublicKey = pKey;
        SecretKey = sKey;
    }
    
    public String toString() {
        return "Chiave pubblica: " + PublicKey.toString() + " Chiave privata: " + SecretKey.toString();
    }
}
