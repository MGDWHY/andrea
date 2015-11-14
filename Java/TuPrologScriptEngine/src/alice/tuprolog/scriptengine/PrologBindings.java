/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import alice.tuprolog.InvalidTheoryException;
import alice.tuprolog.Prolog;
import alice.tuprolog.Theory;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import javax.script.Bindings;

/**
 *
 * @author Andrea
 */
public class PrologBindings implements Bindings {
    
    private Prolog prolog;
    
    private ArrayList<BindingEntry> entries;
    
    public PrologBindings(Prolog prolog) {
        this.prolog = prolog;
        this.entries = new ArrayList<>();
        
        //entries.add(new TheoryBindingEntry(PrologScriptEngine.THEORY, prolog));
    }

    @Override
    public Object put(String key, Object value) {
        
        checkKey(key);
        
        if(value == null)
            throw new NullPointerException("This implementation doesn't allow null values");
        
        int index = getKeyIndex(key);

        if(index == -1) {
            entries.add(new StaticBindingEntry(key, value));
            return null;
        }
        
        BindingEntry e = entries.get(index);
        
        return e.setValue(value);
    }

    @Override
    public void putAll(Map<? extends String, ? extends Object> map) {
        
        if(map == null)
            throw new NullPointerException("Given map is null");
        
        for(String k : map.keySet()) 
            put(k, map.get(k));
    }

    @Override
    public boolean containsKey(Object key) {
        
        checkKey(key);
        
        int index = getKeyIndex(key);
        
        if(index != -1) {
          BindingEntry e = entries.get(index);
          
          if(!e.isDynamic() || e.getValue() != null)
              return true;
             
        }
        return false;
    }

    @Override
    public Object get(Object key) {
        
        checkKey(key);
        
        int index = getKeyIndex(key);
        
        if(index == -1)
            return null;
        
        return entries.get(index).getValue();
        
    }

    @Override
    public Object remove(Object key) {
        
        checkKey(key);
        
        int index = getKeyIndex(key);
        
        if(index == -1)
            return null;
        
        BindingEntry e = entries.get(index);
        
        if(e.isDynamic())
            return e.remove();
        else{
            entries.remove(index);
            return e.getValue();
        }
            
    }

    @Override
    public int size() {
        int size = 0;
        
        for(BindingEntry e : entries) {
            if(!e.isDynamic() || e.getValue() != null)
                size++;
        }
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size() == 0;
    }

    @Override
    public boolean containsValue(Object value) {
        if(value == null)
            return false;
        
        for(BindingEntry e : entries) {
            if(value.equals(e.getValue()))
                return true;
        }
        return false;
    }

    @Override
    public void clear() {
        int index = 0;
        
        Iterator<BindingEntry> it = entries.iterator();
        
        while(it.hasNext()) {
            BindingEntry e = it.next();
            
            if(e.isDynamic())
                e.remove();
            else
                it.remove();
        }
    }

    @Override
    public Set<String> keySet() {
        HashSet<String> result = new HashSet<>();
        
        for(BindingEntry e: entries)
            if(!e.isDynamic() || e.getValue() != null)
                result.add(e.getKey());
        
        return result;
    }

    @Override
    public Collection<Object> values() {
        ArrayList<Object> result = new ArrayList<>();
        for(BindingEntry e : entries)
            if(e.getValue() != null)
                result.add(e.getValue());
        
        return result;
        
    }

    @Override
    public Set<Entry<String, Object>> entrySet() {
        return new HashSet<Entry<String, Object>>(entries);
    }

    public String toString() {
        String str = new String();
        for(BindingEntry e : entries)
            str += e.toString() + "\n";
        
        return str;
    }
    
    private int getKeyIndex(Object key) {
        for(int i = 0; i < entries.size(); i++) {
            if(entries.get(i).getKey().equals(key.toString()))
                return i;
        }
        return -1;       
    }    
    
    private void checkKey(Object key) {
        if(key == null)
            throw new NullPointerException("Key is null");
        
        String s = (String) key;
        
        if(s.isEmpty())
            throw new IllegalArgumentException("Key is an empty string");
    }
    
    private static abstract class BindingEntry implements Map.Entry<String, Object> {
        
        private String key;
        
        public BindingEntry(String key) {
            this.key = key;
        }
        
        public String getKey() {
            return key;
        }
        
        /** returns false if the entry is dynamic */
        public abstract boolean isDynamic();
        
        /** Get the value of this bound object */
        public abstract Object getValue();
        
        /** Set the value of this bound Object. null value must not allowed */
        public abstract Object setValue(Object value);
        
        /** Clear this bound object */
        public abstract Object remove();
        
        public String toString() {
            return "[" + key + "] -> " + getValue();
        }
    }
    
    private static abstract class DynamicBindingEntry extends BindingEntry {
        
        public DynamicBindingEntry(String key) {
            super(key);
        }
        
        public boolean isDynamic() { return true; }
    }    
    
    private static class StaticBindingEntry extends BindingEntry {
        
        private Object value;
        
        public StaticBindingEntry(String key, Object value) {
            super(key);
            setValue(value);
        }

        @Override
        public Object getValue() {
            return value;
        }

        @Override
        public Object setValue(Object value) {
            if(value == null)
                throw new NullPointerException("This implementation doesn't allow null values");            
            
            Object previous = getValue();
            this.value = value;
            return previous;
        }

        @Override
        public Object remove() {
            throw new UnsupportedOperationException("Not supported yet.");
        }

        @Override
        public boolean isDynamic() {
            return false;
        }
        
    }
      
    private static class TheoryBindingEntry extends DynamicBindingEntry {
        
        private Prolog prolog;
        
        public TheoryBindingEntry(String key, Prolog prolog) {
            super(key);
            this.prolog = prolog;
        }
     
        @Override
        public Object getValue() {
            return prolog.getTheory();
        }

        @Override
        public Object setValue(Object value) {
            if(value == null)
                throw new NullPointerException("This implementation doesn't allow null values");
            
            Object previous = getValue();
            
            try {
                prolog.setTheory((Theory)value);
                return previous;
            }
            catch(InvalidTheoryException ex) {
                ex.printStackTrace();
                return null;
        
            }
        }
        
        @Override
        public Object remove() {
            Object previous = getValue();
            prolog.clearTheory();
            return previous;
        }
    }
     
}
