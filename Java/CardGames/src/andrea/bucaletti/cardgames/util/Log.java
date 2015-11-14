/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.util;

/**
 *
 * @author Andrea
 */
public class Log {

    public static void d(String tag, String message) {
        System.out.println("(info - " + tag + ") " + message);
    }

    public static void w(String tag, String message) {
        System.out.println("(warn - " + tag + ") " + message);
    }

    public static void w(String tag, Exception ex) {
        System.out.println("(warn - " + tag + ") " + ex.getClass().getName() + ": " + ex.getMessage());
    }

    public static void e(String tag, String message) {
        System.out.println("(err - " + tag + ") " + message);
    }

    public static void e(String tag, Exception ex) {
        System.out.println("(err - " + tag + ") " + ex.getClass().getName() + ": " + ex.getMessage());
    }
}
