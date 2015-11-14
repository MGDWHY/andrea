/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import alice.tuprolog.Theory;
import alice.tuprolog.scriptengine.PrologScriptEngine;
import alice.tuprologx.ide.JInputFrame;
import java.awt.event.KeyEvent;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

/**
 *
 * @author Andrea
 */
public class Test2 extends javax.swing.JFrame {
    
    
    private JFileChooser fileChooser;
    
    private ScriptEngine scriptEngine;
    private JInputFrame inputFrame;

    /**
     * Creates new form Test2
     */
    public Test2() {
        initComponents();
        
        fileChooser = new JFileChooser();
        
        //ScriptEngineManager manager = new ScriptEngineManager();
        
        //scriptEngine = manager.getEngineByName("tuProlog");

        scriptEngine = new PrologScriptEngine();
        
        inputFrame = new JInputFrame();
        
        scriptEngine.getContext().setReader(new InputStreamReader(inputFrame));
        scriptEngine.getContext().setWriter(txtOutput.getWriter());
    }

 
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        txtScript = new javax.swing.JTextField();
        javax.swing.JScrollPane jScrollPane2 = new javax.swing.JScrollPane();
        txtOutput = new alice.tuprologx.ide.JOutputTextArea();
        jMenuBar1 = new javax.swing.JMenuBar();
        javax.swing.JMenu mnuFile = new javax.swing.JMenu();
        mnuLoadTheory = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        txtScript.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                txtScript_KeyPressed(evt);
            }
        });

        txtOutput.setEditable(false);
        txtOutput.setColumns(20);
        txtOutput.setRows(5);
        jScrollPane2.setViewportView(txtOutput);

        mnuFile.setText("File");

        mnuLoadTheory.setText("Load theory...");
        mnuLoadTheory.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                mnuLoadTheoryActionPerformed(evt);
            }
        });
        mnuFile.add(mnuLoadTheory);

        jMenuBar1.add(mnuFile);

        setJMenuBar(jMenuBar1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 628, Short.MAX_VALUE)
                    .addComponent(txtScript))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 379, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(txtScript, javax.swing.GroupLayout.PREFERRED_SIZE, 29, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void txtScript_KeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txtScript_KeyPressed
        if(evt.getKeyCode() == KeyEvent.VK_ENTER)
            evalScript();
    }//GEN-LAST:event_txtScript_KeyPressed

    private void mnuLoadTheoryActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_mnuLoadTheoryActionPerformed
        // TODO add your handling code here:
        if(fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                Theory theory = new Theory(new FileInputStream(fileChooser.getSelectedFile()));  
                scriptEngine.put(PrologScriptEngine.THEORY, theory);
                printEngineScope();
            }
            catch(IOException ex) {
                JOptionPane.showMessageDialog(this, ex.getMessage(), "IO Exception", JOptionPane.ERROR_MESSAGE);
            }
        }        
    }//GEN-LAST:event_mnuLoadTheoryActionPerformed
    
    private void evalScript() {
        new Thread() {
            public void run() {
                try {
                    scriptEngine.eval(txtScript.getText());
                    printEngineScope();
                    txtOutput.setCaretPosition(txtOutput.getText().length());
                }
                catch(ScriptException ex) {
                    JOptionPane.showMessageDialog(null, ex.getMessage(), "Script Exception", JOptionPane.ERROR_MESSAGE);
                }                
            }
        }.start();
        
    }
    
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
            java.util.logging.Logger.getLogger(Test2.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Test2.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Test2.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Test2.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Test2().setVisible(true);
            }
        });
    }
    
    private void printEngineScope() {
        txtOutput.append("\n-------------------ENGINE SCOPE------------------\n" 
                + scriptEngine.getBindings(ScriptContext.ENGINE_SCOPE).toString()
        		+ "-------------------------------------------------------------\n");           
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem mnuLoadTheory;
    private alice.tuprologx.ide.JOutputTextArea txtOutput;
    private javax.swing.JTextField txtScript;
    // End of variables declaration//GEN-END:variables
}
