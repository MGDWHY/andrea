/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.protocol;

/**
 *
 * @author Andrea
 */
public class Protocol {

    public static final int SERVER_PORT = 10000;
    public static final int CLIENT_PORT = 10001;
    // Messages
    public static final int M_ACK = 200; // ack
    public static final int M_NACK = 201; // negative ack
    public static final int M_ERR = 400; // error
    public static final int M_OK = 401; // ok
    public static final int M_INFO = 300; // info request to the server
    public static final int M_INFOMSG = 300; // info message to the client
    public static final int M_CMD = 500; // command
}
