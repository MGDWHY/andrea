/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package prove;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

/**
 *
 * @author Andrea
 */
public class Client {
    public static void main(String[] args) throws Exception {
        Socket s = new Socket("127.0.0.1", 10000);
        ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
        ObjectInputStream in = new ObjectInputStream(s.getInputStream());

        while(true) {
            out.writeInt(10);
            out.writeObject(new Message(1, "Ciao"));
            Message m = (Message) in.readObject();
        
            System.out.println(m);
            Thread.sleep(1000);
        }
    }
}
