/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.util;

import java.util.ArrayList;

/**
 *
 * @author Andrea
 * A synchronized lockable array list
 */
public class LockableArrayList<E> extends ArrayList<E> {
    
    private static final long serialVersionUID = 5403621343419278968L;
    
    protected int locks = 0;
    
    private ArrayList<E> addList, removeList;
    
    public LockableArrayList() {
        super();
        addList = new ArrayList<E>();
        removeList = new ArrayList<E>();
    }
    
    public synchronized void lock() {
        locks++;
    }
    
    public synchronized void unlock() {
        if(!isLocked())
            throw new IllegalStateException("This list is not locked");
                   
        if(--locks == 0) {

            this.addAll(addList);
            this.removeAll(removeList);

            addList.clear();
            removeList.clear();
        }
    }
    
    public synchronized boolean addUnique(E element) {
        if(isLocked()) {
            if(!(contains(element) || addList.contains(element)))
                return add(element);
            else
                return false;
        } else
            if(!contains(element))
                return add(element);
            else
                return false;
    }
    
    @Override
    public synchronized boolean add(E element) {
        if(isLocked())
            return addList.add(element); 
        else
            return super.add(element);
    }
    
    @Override
    public synchronized boolean remove(Object element) {
        if(isLocked())
            return removeList.add((E)element);
        else
            return super.remove(element);
                    
    }
    
    public boolean isLocked() { return locks > 0; }
}
