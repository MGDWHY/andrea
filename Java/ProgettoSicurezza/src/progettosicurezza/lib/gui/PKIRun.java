/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.gui;

import java.io.IOException;
import progettosicurezza.lib.pki.PKI;
import progettosicurezza.lib.rsa.RSA;
import progettosicurezza.lib.rsa.RSAKeyPair;

/**
 *
 * @author Andrea
 */
public class PKIRun {
    public static void main(String[] args) throws IOException, InterruptedException {
        RSAKeyPair rsaKeys = RSA.GenerateKeys(9733, 10177);
        PKI pki = new PKI(rsaKeys);
        pki.start();
        pki.join();
    }
}
