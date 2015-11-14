/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.util;

import java.nio.charset.Charset;


/**
*
* @author Andrea Bucaletti
*/
public class CircularBuffer {
    
    private int available;
    
    private int readPosition, writePosition;
    
    private byte[] data;
    
    private int dimension;
    
    public CircularBuffer(int dim) {
        available = readPosition = writePosition = 0;
        dimension = dim;
        data = new byte[dim];
    }
    
    public int available() {
        return available;
    }
    
    public byte get() {

        while(available == 0) {
//            printState("Wating in get");
            doWait();
        }
        
        byte result = data[readPosition];
        readPointer(1);
        doNotifyAll();
        return result;
    }
    
    public void put(byte b) {
        while(available == (dimension - 1)) {
//            printState("Wating in put");
            doWait(); 
        }
        data[writePosition] = b;
        writePointer(1);
        doNotifyAll();
    }
    
    public  void put(byte[] bytes) {
        for(byte b : bytes)
            put(b);
    }
    
    public void putString(String s) {
        put(s.getBytes());
    }
    
    public void putString(String s, Charset c) {
        put(s.getBytes(c));
    }
    
    public void putChar(char c) {    
        put((byte)(c & 0x00FF));
    }
    
    public char getChar() {
        return (char)(0xFFFF & (get() << 8) & get());
    }
    
    private void writePointer(int amount) {
        writePosition = (writePosition += amount) % dimension;
        available += amount;
    }
    
    private void readPointer(int amount) {
        readPosition = (readPosition += amount) % dimension;
        available -= amount;
    }
    
    /*
    private void printState(String state) {
        System.out.println("State: " + state + ", Positions: " +  readPosition + " " + writePosition + ", Available: " + available);
    }
    */
    private synchronized void doWait() {
        try { wait();} 
        catch (InterruptedException ex) { ex.printStackTrace();}
    }
    
    private synchronized void doNotifyAll() {
        notifyAll();
    }
}
