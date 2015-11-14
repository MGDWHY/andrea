/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprologx.ide;

import alice.util.CircularBuffer;
import java.awt.BorderLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;

import javax.imageio.IIOException;
import javax.swing.JFrame;
import javax.swing.JTextField;

/**
 *
 * @author Andrea Bucaletti
 */
public class JInputFrame extends Reader implements KeyListener {
	
	protected Object lock;
    
    protected boolean closed;
    
    protected StringBuffer buffer;
    
    protected JFrame frame;
    protected JTextField inputText;
    
    boolean endOfStream = false;
    
    public JInputFrame() {
        
    	lock = new Object();
    	
        buffer = new StringBuffer();
        
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
    public boolean ready() {
    	return buffer.length() > 0;
    }
    
    @Override
    public void close() {
        closed = true;
    }
    

    @Override
    public void keyTyped(KeyEvent ke) {
   
    }

    @Override
    public void keyPressed(KeyEvent ke) {
        if(ke.getKeyCode() == KeyEvent.VK_ENTER) {
            buffer.append(inputText.getText() + '\n');
            inputText.setText("");
            frame.setVisible(false);
            synchronized(lock) { lock.notifyAll(); }
        }
    }

    @Override
    public void keyReleased(KeyEvent ke) {
        
    }

	@Override
	public int read(char[] buf, int off, int len) throws IOException {
		synchronized (lock) {
						
	        if(closed)
	            throw new IOException("Stream closed");
	        
	        frame.setVisible(true);		
			
			while(buffer.length() == 0) {
				try { lock.wait(); }
				catch(InterruptedException ex) {}
			}
			
			int r = 0;
			
			for(; r < len && buffer.length() > 0; r++) {
				buf[r + off] = buffer.charAt(r);
				buffer.deleteCharAt(r);
			}
			
			return r;
		}
	}

}
