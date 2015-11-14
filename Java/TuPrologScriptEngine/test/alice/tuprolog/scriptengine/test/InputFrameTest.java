/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine.test;

import alice.tuprologx.ide.JInputFrame;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

/**
 *
 * @author Andrea
 */
public class InputFrameTest {
    public static void main(String[] args) throws IOException, InterruptedException {
        final JInputFrame f = new JInputFrame();
        InputStreamReader is = new InputStreamReader(f);
        Reader rd = (Reader) is;
        
        while(true) {
            System.out.print(is.read());
        }
        /*
        ByteBuffer b = ByteBuffer.allocate(1000);
        b.position(0);
        b.asFloatBuffer().put(1);
        System.out.println(b.asFloatBuffer().get());*/      
    }
}
