/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import alice.tuprolog.Prolog;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.Reader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import javax.script.Bindings;
import javax.script.ScriptContext;
import javax.script.SimpleBindings;

/**
*
* @author Andrea Bucaletti
* 
*/
public class PrologScriptContext implements ScriptContext {
    
    private static List<Integer> scopes;
    
    static {
        scopes = new ArrayList<>();
        scopes.add(ENGINE_SCOPE);
        scopes.add(GLOBAL_SCOPE);
        scopes = Collections.unmodifiableList(scopes);
    }
    
    protected Prolog prolog;
    protected Bindings engineScope, globalScope;
    
    protected Reader reader;
    protected Writer writer, errorWriter;
     
    public PrologScriptContext(Prolog prolog) {
        this.prolog = prolog;
           
        engineScope = new PrologBindings(prolog);
        globalScope = null;     
        
        reader = new InputStreamReader(System.in);
        writer = new PrintWriter(System.out);
        errorWriter = new PrintWriter(System.err);
    }

    @Override
    public void setBindings(Bindings bndngs, int scope) {
        
        switch(scope) {
            case ENGINE_SCOPE:
                if(bndngs == null)
                    throw new NullPointerException("Local scope bindings is null");

                engineScope = bndngs;      
                return;
            case GLOBAL_SCOPE:
                globalScope = bndngs;
                return;
            default:
                throw new IllegalArgumentException("Invalid scope value");                
                
                
        }
    }

    @Override
    public Bindings getBindings(int scope) {
        
        switch(scope) {
            case ENGINE_SCOPE:
                return engineScope;
            case GLOBAL_SCOPE:
                return globalScope;
            default:
                throw new IllegalArgumentException("Invalid scope value");
        }
        
    }

    @Override
    public void setAttribute(String name, Object value, int scope) {
        switch(scope) {
            case ENGINE_SCOPE:
                engineScope.put(name, value);
                return;
            case GLOBAL_SCOPE:
                if(globalScope != null)
                    globalScope.put(name, value);
                return;
            default:
                throw new IllegalArgumentException("Invalid scope value");
        }
    }

    @Override
    public Object getAttribute(String name, int scope) {
        switch(scope) {
            case ENGINE_SCOPE:
                return engineScope.get(name);
            case GLOBAL_SCOPE:
                if(globalScope != null)
                    return globalScope.get(name);
                return null;
            default:
                throw new IllegalArgumentException("Invalid scope value");
        }
    }

    @Override
    public Object removeAttribute(String name, int scope) {
        switch(scope) {
            case ENGINE_SCOPE:
                if (getBindings(ENGINE_SCOPE) != null) {
                   return getBindings(ENGINE_SCOPE).remove(name);
                }
                return null;

            case GLOBAL_SCOPE:
                if (getBindings(GLOBAL_SCOPE) != null) {
                    return getBindings(GLOBAL_SCOPE).remove(name);
                }
            default:
                throw new IllegalArgumentException("Illegal scope value.");
        }
    }

    @Override
    public Object getAttribute(String name) {
        if (engineScope.containsKey(name)) {
            return getAttribute(name, ENGINE_SCOPE);
        } else if (globalScope != null && globalScope.containsKey(name)) {
            return getAttribute(name, GLOBAL_SCOPE);
        }
        return null;
    }

    @Override
    public int getAttributesScope(String name) {
        if (engineScope.containsKey(name)) {
            return ENGINE_SCOPE;
        } else if (globalScope != null && globalScope.containsKey(name)) {
            return GLOBAL_SCOPE;
        } else {
            return -1;
        }
    }

    @Override
    public Writer getWriter() {
        return writer;
    }

    @Override
    public Writer getErrorWriter() {
        return errorWriter;
    }

    @Override
    public void setWriter(Writer w) {
        if(w == null)
            throw new NullPointerException("Writer is null");
        
        this.writer = w;
    }

    @Override
    public void setErrorWriter(Writer ew) {
        if(ew == null)
            throw new NullPointerException("Error writer is null");
        
        this.errorWriter = ew;
    }

    @Override
    public Reader getReader() {
        return reader;
    }

    @Override
    public void setReader(Reader r) {
        if(r == null)
            throw new NullPointerException("Reader is null");
        
        this.reader = r;
    }

    @Override
    public List<Integer> getScopes() {
        return PrologScriptContext.scopes;
    }
    
}
