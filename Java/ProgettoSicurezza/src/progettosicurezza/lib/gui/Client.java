/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package progettosicurezza.lib.gui;

import java.awt.event.KeyEvent;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.NoSuchAlgorithmException;
import javax.swing.JOptionPane;
import progettosicurezza.lib.cfb.CFBCipher;
import progettosicurezza.lib.dh.DiffieHellman;
import progettosicurezza.lib.pki.Certificate;
import progettosicurezza.lib.pki.PKI;
import progettosicurezza.lib.rsa.RSA;
import progettosicurezza.lib.rsa.RSAKeyPair;
import progettosicurezza.libutil.Utility;
import progettosicurezza.test.TestRSA;

/**
 *
 * @author Andrea
 */
public class Client extends javax.swing.JFrame implements Runnable {

    private static final int DEFAULT_PORT = 8000;
    
    private Certificate certificate;
    private ServerSocket serverSocket;
    private RSAKeyPair rsaKeys;
    private int listeningPort;
    
    private static ServerSocket CreateServerSocket() {
        ServerSocket sSocket = null;
        boolean done = false;
        int port = DEFAULT_PORT;
        
        while(!done) {
            try {
                sSocket = new ServerSocket(port++);
                done = true;
            }
            catch(Exception ex) {}
        }
        
        return sSocket;
    }
    
    /**
     * Creates new form Client
     */
    
    
    public Client() {
        initComponents();
        GenerateKeys();
        serverSocket = CreateServerSocket();
        listeningPort = serverSocket.getLocalPort();
        System.out.println("Client listening on port: " + listeningPort);
        new Thread(this).start();
    }
    
    public static void println(String s) {
        System.out.println(s);
    }
    
    public void run() {
        while(true) {
            
            try {
                DiffieHellman dh = new DiffieHellman();
                Socket socket = serverSocket.accept();
                DataInputStream in = new DataInputStream(socket.getInputStream());
                DataOutputStream out = new DataOutputStream(socket.getOutputStream());
                             
                // Read parameters
                
                Certificate cert = Certificate.read(in);
                               
                byte[] dataIn = new byte[128];
                for(int i = 0; i < dataIn.length; i++)
                    dataIn[i] = in.readByte();
                           
                byte[] plainText = RSA.ArrayDecipher8(rsaKeys.SecretKey, dataIn);
                
                DataInputStream din = new DataInputStream(new ByteArrayInputStream(plainText));
                
                long yOther = din.readLong();
                long r2 = din.readLong();
                
                // Write parameters
                
                ByteArrayOutputStream osCipher = new ByteArrayOutputStream();
                DataOutputStream dosCipher = new DataOutputStream(osCipher);
                
                dosCipher.writeLong(dh.GetY());
                dosCipher.writeLong(dh.GetFirstRandom());
                
                byte[] dataOut = RSA.ArrayCipher8(cert.PublicKey, osCipher.toByteArray());
                
                out.write(dataOut);
                
                // Generating key
                dh.SetSecondRandom(r2);
                byte[] key = dh.GenerateKey(yOther);

                // Receive message
                ByteArrayOutputStream buf = new ByteArrayOutputStream();
                CFBCipher cipher = new CFBCipher(key);
                
                int r;
                while((r = in.read()) != -1)
                    buf.write((byte)r);
                
                plainText = cipher.Decipher(buf.toByteArray());
                
                txtMessages.append(cert.UserID + " - " + new String(plainText) + "\n");

                socket.close();
              
            }
            catch(Exception ex) {
                ex.printStackTrace();
            }

            
            
        }
    }
    
    private void SendMessage() {
        Certificate sendTo = (Certificate) cmoCertifiedUsers.getSelectedItem();
        DiffieHellman dh = new DiffieHellman();
        try {

            Socket socket = new Socket("127.0.0.1", sendTo.ListeningPort);
            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            
            // Write parameters
            
            Certificate.write(out, certificate);
            
            
            ByteArrayOutputStream osCipher = new ByteArrayOutputStream();
            DataOutputStream dosCipher = new DataOutputStream(osCipher);

            dosCipher.writeLong(dh.GetY());
            dosCipher.writeLong(dh.GetFirstRandom());

            byte[] dataOut = RSA.ArrayCipher8(sendTo.PublicKey, osCipher.toByteArray());
           
            
            out.write(dataOut);
            
            // Read Paramters

            byte[] dataIn = new byte[128];
            for(int i = 0; i < dataIn.length; i++)
                dataIn[i] = in.readByte();

            byte[] plainText = RSA.ArrayDecipher8(rsaKeys.SecretKey, dataIn);

            DataInputStream din = new DataInputStream(new ByteArrayInputStream(plainText));

            long yOther = din.readLong();
            long r2 = din.readLong();
            
            // Generating key
            dh.SetSecondRandom(r2);
            byte[] key = dh.GenerateKey(yOther);
            
            // Sending message
            CFBCipher cipher = new CFBCipher(key);
            out.write(cipher.Cipher(txtMsg.getText().getBytes()));
            
            socket.close();
            
            txtMsg.setText("");
            
        }
        catch(Exception ex) { 
            ex.printStackTrace();
        }
        
    }    
    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        txtP = new javax.swing.JTextField();
        txtQ = new javax.swing.JTextField();
        lblP = new javax.swing.JLabel();
        lblP1 = new javax.swing.JLabel();
        cmdGenerateKeys = new javax.swing.JButton();
        cmdRequestCertificate = new javax.swing.JButton();
        lblKey = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        txtMessages = new javax.swing.JTextArea();
        cmoCertifiedUsers = new javax.swing.JComboBox();
        lblSendMessage = new javax.swing.JLabel();
        txtMsg = new javax.swing.JTextField();
        cmdUpdateUsers = new javax.swing.JButton();
        lblUserID = new javax.swing.JLabel();
        txtUserID = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Client");
        setResizable(false);

        txtP.setText("9733");

        txtQ.setText("10177");

        lblP.setText("P");

        lblP1.setText("Q");

        cmdGenerateKeys.setText("Genera chiavi");
        cmdGenerateKeys.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmdGenerateKeysActionPerformed(evt);
            }
        });

        cmdRequestCertificate.setText("Richiedi certificato");
        cmdRequestCertificate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmdRequestCertificateActionPerformed(evt);
            }
        });

        lblKey.setText("Chiavi");

        txtMessages.setEditable(false);
        txtMessages.setColumns(20);
        txtMessages.setRows(5);
        jScrollPane1.setViewportView(txtMessages);

        cmoCertifiedUsers.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmoCertifiedUsersActionPerformed(evt);
            }
        });

        lblSendMessage.setText("Invia messaggio a");

        txtMsg.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                txtMsgKeyPressed(evt);
            }
        });

        cmdUpdateUsers.setText("Aggiorna utenti");
        cmdUpdateUsers.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cmdUpdateUsersActionPerformed(evt);
            }
        });

        lblUserID.setText("ID Utente");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jScrollPane1)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(lblUserID)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtUserID, javax.swing.GroupLayout.DEFAULT_SIZE, 287, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(cmdRequestCertificate)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cmdUpdateUsers, javax.swing.GroupLayout.PREFERRED_SIZE, 119, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(lblKey, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(lblP)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtP, javax.swing.GroupLayout.PREFERRED_SIZE, 74, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblP1, javax.swing.GroupLayout.PREFERRED_SIZE, 11, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtQ, javax.swing.GroupLayout.PREFERRED_SIZE, 74, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cmdGenerateKeys, javax.swing.GroupLayout.PREFERRED_SIZE, 119, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(lblSendMessage)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cmoCertifiedUsers, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txtMsg)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txtP, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lblP)
                    .addComponent(lblP1)
                    .addComponent(txtQ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmdGenerateKeys))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lblKey)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lblUserID)
                    .addComponent(txtUserID, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmdRequestCertificate)
                    .addComponent(cmdUpdateUsers))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txtMsg, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmoCertifiedUsers, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lblSendMessage))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 262, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void cmdGenerateKeysActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdGenerateKeysActionPerformed
        GenerateKeys();
    }//GEN-LAST:event_cmdGenerateKeysActionPerformed

    private void cmoCertifiedUsersActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmoCertifiedUsersActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_cmoCertifiedUsersActionPerformed

    private void cmdRequestCertificateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdRequestCertificateActionPerformed
        RequestCertificate();
    }//GEN-LAST:event_cmdRequestCertificateActionPerformed

    private void cmdUpdateUsersActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cmdUpdateUsersActionPerformed
        RequestAllCertificates();
    }//GEN-LAST:event_cmdUpdateUsersActionPerformed

    private void txtMsgKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txtMsgKeyPressed
        if(evt.getKeyCode() == KeyEvent.VK_ENTER) {
            SendMessage();
        }        // TODO add your handling code here:
    }//GEN-LAST:event_txtMsgKeyPressed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Client.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Client.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Client.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Client.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Client().setVisible(true);
            }
        });
    }
    
    private void GenerateKeys() {
        try {
            int p = Integer.parseInt(txtP.getText());
            int q = Integer.parseInt(txtQ.getText());
            rsaKeys = RSA.GenerateKeys(p, q);
            lblKey.setText(rsaKeys.toString() + " (non certificata)");
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }
    
    private void RequestCertificate() {
        Certificate cert = new Certificate(txtUserID.getText(), rsaKeys.PublicKey, listeningPort);
        cert.Sign(rsaKeys.SecretKey);
        try {
            Socket socket = new Socket("127.0.0.1", PKI.LISTEN_PORT);
            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            
            out.writeInt(PKI.CMD_AUTH_REQUEST);
            Certificate.write(out, cert);
            int result = in.readInt();
            
            if(result == PKI.CMD_OK) {
                certificate = Certificate.read(in);
                lblKey.setText(rsaKeys.toString() + " (certificata)");
                cmdGenerateKeys.setEnabled(false);
                cmdRequestCertificate.setEnabled(false);
            } else {
                JOptionPane.showMessageDialog(this, "Registration failed!");
            }
            
            socket.close();
            
        }
        catch(IOException ex) {
            ex.printStackTrace();
        }
    }
    

    
    private void RequestAllCertificates() {
        cmoCertifiedUsers.removeAllItems();
        
        try {
            Socket socket = new Socket("127.0.0.1", PKI.LISTEN_PORT);
            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            
            out.writeInt(PKI.CMD_GET_ALL_CERTIFICATES);
            int num = in.readInt();
            System.out.println(num);
            for(int i = 0; i < num; i++) {
                Certificate cert = Certificate.read(in);
                cmoCertifiedUsers.addItem(cert);
                
            }
            socket.close();
        }
        catch(IOException ex) {
            ex.printStackTrace();
        }        
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton cmdGenerateKeys;
    private javax.swing.JButton cmdRequestCertificate;
    private javax.swing.JButton cmdUpdateUsers;
    private javax.swing.JComboBox cmoCertifiedUsers;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblKey;
    private javax.swing.JLabel lblP;
    private javax.swing.JLabel lblP1;
    private javax.swing.JLabel lblSendMessage;
    private javax.swing.JLabel lblUserID;
    private javax.swing.JTextArea txtMessages;
    private javax.swing.JTextField txtMsg;
    private javax.swing.JTextField txtP;
    private javax.swing.JTextField txtQ;
    private javax.swing.JTextField txtUserID;
    // End of variables declaration//GEN-END:variables
}
