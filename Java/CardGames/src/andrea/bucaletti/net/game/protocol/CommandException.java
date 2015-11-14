/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.protocol;

/**
 *
 * @author Andrea
 */
public class CommandException extends Exception {

    /**
     * Creates a new instance of
     * <code>CommandException</code> without detail message.
     */
    public CommandException() {
    }

    /**
     * Constructs an instance of
     * <code>CommandException</code> with the specified detail message.
     *
     * @param msg the detail message.
     */
    public CommandException(String msg) {
        super(msg);
    }
}
