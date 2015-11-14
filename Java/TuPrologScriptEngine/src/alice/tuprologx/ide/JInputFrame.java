/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprologx.ide;

import alice.tuprolog.util.CircularBuffer;
import java.awt.BorderLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.io.InputStream;
import javax.imageio.IIOException;
import javax.swing.JFrame;
import javax.swing.JTextField;

/**
 *
 * @author Andrea
 */
public class JInputFrame extends InputStream implements KeyListener {
    
    protected boolean closed;
    
    protected CircularBuffer buffer;
    
    protected JFrame frame;
    protected JTextField inputText;
    
    boolean endOfStream = false;
    
    public JInputFrame() {
        
        buffer = new CircularBuffer(1024);
        
        frame = new JFrame();
        
        inputText = new JTextField(20);
        inputText.addKeyListener(this);
        
        frame.setLayout(new BorderLayout());
        frame.getContentPane().add(BorderLayout.CENTER, inputText);
        frame.pack();
        frame.setResizable(false);
        frame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        
    }
    
    @Override
    public int available() {
        return buffer.available();
    }
    
    @Override
    public void close() {
        closed = true;
    }
    
    
    @Override
    public int read() throws IOException {
        
        if(closed)
            throw new IOException("Stream closed");
        
        frame.setVisible(true);
        if(endOfStream && buffer.available() == 0) {
            frame.setVisible(false);
            endOfStream = false;
            return -1;
        }
        return 0x000000FF & buffer.get();
    }

    @Override
    public void keyTyped(KeyEvent ke) {
   
    }

    @Override
    public void keyPressed(KeyEvent ke) {
        if(ke.getKeyCode() == KeyEvent.VK_ENTER) {
            buffer.putString(inputText.getText());
            inputText.setText("");
            endOfStream = true;
        }
    }

    @Override
    public void keyReleased(KeyEvent ke) {
        
    }

}
