/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.util;

/**
 *
 * @author Andrea
 */
public class ConsoleLog implements Log {

    @Override
    public void i(Object caller, String message) {
        System.out.println("(I)" + caller.toString() + ": " + message);
    }

    @Override
    public void w(Object caller, String message) {
        System.out.println("(W)" + caller.toString() + ": " + message);
    }

    @Override
    public void e(Object caller, String message) {
        System.out.println("(E)" + caller.toString() + ": " + message);

    }
    
}
