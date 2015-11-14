/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.util;

/**
 *
 * @author Andrea
 */
public interface Log {
    public void i(Object caller, String message);
    public void w(Object caller, String message);
    public void e(Object caller, String message);
}
