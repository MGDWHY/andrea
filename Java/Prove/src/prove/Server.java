/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package prove;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 *
 * @author Andrea
 */
public class Server {
    public static void main(String args[]) throws Exception {
        ServerSocket ss = new ServerSocket(10000);
        Socket s = ss.accept();
        ObjectInputStream in = new ObjectInputStream(s.getInputStream());
        ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
        
        while(true) {
            in.readInt();
            Message m = (Message) in.readObject();

            System.out.println(m);

            out.writeObject(m);
        }
        
        
    }
}
