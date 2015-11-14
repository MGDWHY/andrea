/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import java.util.Arrays;
import java.util.List;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;

/**
 *
 * @author Andrea
 */
public class PrologScriptEngineFactory implements ScriptEngineFactory {
    
    public static final PrologScriptEngineFactory DEFAULT_FACTORY;
    
    private static final String ENGINE_NAME = "tuProlog";
    private static final String ENGINE_VERSION = "1.0";
    
    private static final String LANGUAGE_NAME = "prolog";
    private static final String LANGUAGE_VERSION = "1.0";
    
    private static final List<String> EXTENSIONS = Arrays.asList("pro", "pl", "2p");
    private static final List<String> MIME_TYPES = Arrays.asList("text/plain");
    private static final List<String> NAMES = Arrays.asList("tuProlog", "prolog");
    
    static {
        DEFAULT_FACTORY = new PrologScriptEngineFactory();
    }
    

    @Override
    public String getEngineName() {
        return ENGINE_NAME;
    }

    @Override
    public String getEngineVersion() {
        return ENGINE_VERSION;
    }

    @Override
    public List<String> getExtensions() {
        return EXTENSIONS;
    }

    @Override
    public List<String> getMimeTypes() {
        return MIME_TYPES;
    }

    @Override
    public List<String> getNames() {
        return NAMES;
    }

    @Override
    public String getLanguageName() {
        return LANGUAGE_NAME;
    }

    @Override
    public String getLanguageVersion() {
        return LANGUAGE_VERSION;
    }

    @Override
    public Object getParameter(String string) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public String getMethodCallSyntax(String obj, String methodName, String... args) {
        String result = new String();
        
        result += obj + " <- " + methodName;
        
        if(args.length > 0) {
            result += "(";
            
            for(int i = 0; i < args.length; i++) {
                result += args[i] + (i == args.length - 1 ? ")" : ", ");
            }
        }
        
        return result;
    }

    @Override
    public String getOutputStatement(String string) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public String getProgram(String... strings) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public ScriptEngine getScriptEngine() {
        return new PrologScriptEngine();
    }
}
