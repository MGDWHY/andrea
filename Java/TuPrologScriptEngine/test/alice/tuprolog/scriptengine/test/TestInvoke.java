/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine.test;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.reflect.Method;

/**
 *
 * @author Andrea
 */
public class TestInvoke extends Thread {
    private int counter = 0;
    
    
    public void run() {
        try {
            MethodType mt = MethodType.methodType(long.class, int.class, int.class);
            MethodHandle mh = MethodHandles.lookup().findVirtual(this.getClass(), "testMethod", mt);            
            while(true) {
                mh.invoke(this, 1, 1);
                counter++;
            }
        }
        catch(Throwable ex) { 
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
        TestInvoke t = new TestInvoke();
        t.start();
        Thread.sleep(15000);
        t.interrupt();
        System.out.println("Iterations: "  + t.getCounter());
    }    
}
