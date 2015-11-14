/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectcolossus.util;

import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

/**
 *
 * @author Andrea
 */
public class Log {
    
    private String log;
    private String tag;
    private Calendar calendar;
    
    public Log(String tag) {
        log = new String();
        calendar = GregorianCalendar.getInstance();
        this.tag = tag;
    }
    
    public Log(Class clazz) {
        this(clazz.getCanonicalName());
    }    
    
    public void i(String str) {
        message(str, "INFO");
    }
    
    public void w(String str) {
        message(str, "WARN");
    }
    
    public void e(String str) {
        message(str, "ERR");
    }
    
    public String getLog() {
        return log;
    }
    
    private void message(String str, String qualifier) {
        log += "[" + getCurrentTimeStamp() + "] " + tag + "(" + qualifier + ") - " + str + "\n";
    }
    
    private String getCurrentTimeStamp() {
        return new Timestamp(calendar.getTime().getTime()).toString();
    }
}
