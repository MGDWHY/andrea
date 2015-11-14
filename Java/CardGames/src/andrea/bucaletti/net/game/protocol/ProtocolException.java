/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.protocol;

/**
 *
 * @author Andrea
 */
public class ProtocolException extends Exception {

    /**
     * Creates a new instance of
     * <code>ProtocolException</code> without detail message.
     */
    public ProtocolException() {
    }

    /**
     * Constructs an instance of
     * <code>ProtocolException</code> with the specified detail message.
     *
     * @param msg the detail message.
     */
    public ProtocolException(String msg) {
        super(msg);
    }
}
