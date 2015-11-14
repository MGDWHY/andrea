/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.pki;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import progettosicurezza.lib.rsa.RSAKeyPair;

/**
 *
 * @author Andrea
 */
public class PKI extends Thread {
    
    public static final String HASH_ALGORITHM = "MD5";
    
    public static final int LISTEN_PORT = 10000;
    
    public static final int CMD_AUTH_REQUEST = 100;
    public static final int CMD_VERIFY_CERTIFICATE = 101;
    public static final int CMD_GET_ALL_CERTIFICATES = 102;
    public static final int CMD_OK = 200;
    public static final int CMD_BADREQUEST = 400;
    
    private ArrayList<Certificate> certificates;
    private ServerSocket serverSocket;
    private RSAKeyPair rsaKeys;
    
    
    public PKI(RSAKeyPair keys) throws IOException {
        serverSocket = new ServerSocket(LISTEN_PORT);
        rsaKeys = keys;
        certificates = new ArrayList<Certificate>();
    }
    
    public void run() {
        while(Thread.currentThread().isAlive()) {
            try {
                Socket client = serverSocket.accept();
                
                processRequest(client);
            
            }
            catch(IOException ex) {
                ex.printStackTrace();
            }
        }
    }
    
    private void processRequest(Socket client) throws IOException  {
        DataInputStream is = new DataInputStream(client.getInputStream());
        DataOutputStream os = new DataOutputStream(client.getOutputStream());
        
        int cmd = is.readInt();
        Certificate request;
        switch(cmd) {
            case CMD_AUTH_REQUEST:
                request = Certificate.read(is);
                if(request.Verify()) { // POP 
                    request.Sign(rsaKeys.SecretKey);
                    certificates.add(request);
                    os.writeInt(CMD_OK);
                    Certificate.write(os, request);
                } else
                    os.writeInt(CMD_BADREQUEST);
                break;
            case CMD_VERIFY_CERTIFICATE:
                request = Certificate.read(is);
                
                if(request.Verify(rsaKeys.PublicKey))    
                    os.writeInt(CMD_OK);
                else
                    os.writeInt(CMD_BADREQUEST);
                
                break;
            case CMD_GET_ALL_CERTIFICATES:
                os.writeInt(certificates.size());
                for(int i = 0; i < certificates.size(); i++)
                    Certificate.write(os, certificates.get(i));
                break;
            default:
                os.writeInt(CMD_BADREQUEST);
        }
        
        client.close();
    }
    
    private boolean AddCertificate(Certificate cert) {
        for(int i = 0; i < certificates.size(); i++)
            if(certificates.get(i).UserID.contains(cert.UserID))
                return false;
        
        certificates.add(cert);
        return true;
    }
}
