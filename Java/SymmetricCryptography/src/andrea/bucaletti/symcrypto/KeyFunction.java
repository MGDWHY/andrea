/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.symcrypto;

import java.io.InputStream;

/**
 *
 * @author Andrea
 */
public abstract class KeyFunction<T> extends Function {
    
    private T key;
    
    public KeyFunction(InputStream in) { super(in); }
    
    public void setKey(T key) {
        if(acceptKey(key))
            this.key = key;
        else
            throw new IllegalArgumentException("Illgal key: " + key);
    }
    
    public T getKey() { return key; }
    
    public abstract boolean acceptKey(T key);
    
}
