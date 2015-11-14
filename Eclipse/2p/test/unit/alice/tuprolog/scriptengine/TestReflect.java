/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import java.lang.reflect.Method;

/**
 *
 * @author Andrea
 */
public class TestReflect extends Thread {
    
    private int counter = 0;
    
    
    public void run() {
        try {
            Class[] types = {int.class, int.class};
            int vals[] = {1,1};
            Method me = this.getClass().getDeclaredMethod("testMethod", types);
            while(true) {
                long r = (Long)me.invoke(this, 1, 1);
                counter++;
            }
        }
        catch(Exception ex) { 
            ex.printStackTrace(); 
            interrupt();
        }
    }
    
    public int getCounter() { return counter; }
    
    public long testMethod(int a, int b) {
        long x = 0;
        
        for(int i = 0; i < 100; i++)
            x +=(i + a + b);
        
        return x;
        
    }
    
    public static void main(String[] args) throws InterruptedException {
        TestReflect t = new TestReflect();
        t.start();
        Thread.sleep(15000);
        t.interrupt();
        System.out.println("Iterations: "  + t.getCounter());
    }
    
}
