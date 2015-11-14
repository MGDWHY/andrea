/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprologx.ide;

import java.io.IOException;
import java.io.StringWriter;
import java.io.Writer;
import javax.swing.JTextArea;

/**
 *
 * @author Andrea
 */
public class JOutputTextArea extends JTextArea {
    
    private TextAreaWriter writer;
    
    public JOutputTextArea() {
        writer = new TextAreaWriter(this);
    }
    
    public Writer getWriter() { return writer; }
    
    private static class TextAreaWriter extends Writer {
        
        private boolean closed;
        
        private JOutputTextArea textArea;
        
        public TextAreaWriter(JOutputTextArea textArea) {
            this.textArea = textArea;
            this.closed = false;
        }
        
        @Override
        public void write(char[] chars, int off, int len) throws IOException {
            
            if(closed)
                throw new IOException("Stream closed");
            
            String str = new String();
            
            for(int k = off; k < off + len; k++)
                str += chars[k];
            
            textArea.append(str);
        }
        
        /* No need to flush this */
        @Override
        public void flush() throws IOException {
            if(closed)
                throw new IOException("Stream closed");        
        }

        @Override
        public void close() throws IOException {
            closed = true;
        }
        
    }
    
}
