/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.scriptengine;

import alice.tuprolog.InvalidLibraryException;
import alice.tuprolog.InvalidTheoryException;
import alice.tuprolog.MalformedGoalException;
import alice.tuprolog.NoMoreSolutionException;
import alice.tuprolog.NoSolutionException;
import alice.tuprolog.Prolog;
import alice.tuprolog.SolveInfo;
import alice.tuprolog.Struct;
import alice.tuprolog.Theory;
import alice.tuprolog.Var;
import alice.tuprolog.event.ExceptionEvent;
import alice.tuprolog.event.ExceptionListener;
import alice.tuprolog.lib.InvalidObjectIdException;
import alice.tuprolog.lib.JavaLibrary;
import alice.tuprolog.lib.SimpleIOLibrary;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import javax.script.Bindings;
import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;
import javax.script.ScriptException;


/*
 * Da quanto si legge sulla specifica, questa classe rappresenta praticamente 
 * l'interfaccia dell'interprete. In particolare fornisce vari metodi per 
 * eseguire degli script, e per definirne il contesto (vedi dopo).
 * 
 * METODO eval(..)
 * eseguono lo script passato come String, o da leggere da un Reader, ed 
 * eventualmente un contesto di esecuzione (vedi CONTEXT) e restiuiscono 
 * un generico Object come risultato, sul quale non c'è alcun vincolo 
 * (può essere qualsiasi cosa insomma)
 * 
 * CONTEXT
 * In questa specifica ogni ScriptEngine ha un Context di default 
 * (defaultContext), che è un oggetto che implementa l'interfaccia 
 * ScriptContext. Uno script context ha delle tabelle (Map<String, Object>)
 * che associano appunto Stringhe ad oggetti. Ogni tabella rappresenta 
 * un preciso SCOPE.
 * 
 * SCOPES:
 * di default, l'interfaccia ScriptContext mette a disposizione 2 scopes 
 * principali:
 * - ENGINE SCOPE - Visibile soltanto allo ScriptEngine che lo utilizza. 
 * Alcune chiavi sono riservate (in paritcolare le chiavi nella forma
 * "javax.script.XXX"). Queste sono utilizzate per inserire dei metadati quali, 
 * nome dell'engine, nome del linguaggio, versione, ecc.
 * 
 * La specifica dice anche che, se gli oggetti nella mappa non hanno un 
 * significato particolare, ome ad esempio quelli citati sopra, DOVREBBERO 
 * (cosi è scritto) essere visibili come variabili allo script durante 
 * la sua esecuzione.
 * 
 * In pratica si tratta appunto di fare un binding tra gli oggetti presenti 
 * nella mappa e il linguaggio  di scripting nel seguente modo:
    * - i valori di tali oggetti dovrebbero essere accedibili da uno script. 
    * Quindi se l'applicazione host inserisce un oggetto nell'engine scope,
    * questo deve essere accedibile dallo script
    * - L'applicazione host deve essere in grado di accedere poi ai bindings e 
    * ritrovare gli oggetti, eventualmente anche modificati dallo script stesso.
    * 
 * - GLOBAL SCOPE - Questo scope è visibile a tutti gli ScriptEngine creati 
 * dallo stesso ScriptManager. Nell'implementazione di default di ScriptContext
 * (SimpleScriptContext) fornita dalla specifica, questo valore viene 
 * inizializzato a null, e cosi ho fatto anche io. Si presume che il global 
 * scope sia fornito eventualmente dall'applicazione host stessa.
 * E' possibile rendere visibili per gli script anche gli oggetti inseriti 
 * nel global context, ma non c'è nessun obbilgo.
 * 
 * - ALTRI SCOPES - è possibile definire altri scope se il particolare linguaggio di scripting lo richiede.
 * 
 * INPUT/OUTPUT
 * La specifica giustamente presuppone che uno ScriptContext abbia un proprio 
 * input, output e error. Questi streams sono quelli che appunto LO SCRIPT usa 
 * come standard input, output e error. Un'implementazione di ScriptContext 
 * dovrebbe permettere l'accesso e la modifica di questi stream (nel senso, 
 * voglio cambiare lo standard input usato dal mio script? utilizzo il metodo 
 * setWriter del context).
 * 
 * INTERFACCIAMENTO CON TUPROLOG
 * Un'istanza di PrologScriptEngine crea internamente un oggetto Prolog. Questo 
 * è utilizzato per la risuluzione di tutti gli script passati ai metodi eval.
 * 
 * Nei metodi eval si richiama il metodo Prolog.solve() per l'esecuzione 
 * dello script.
 * 
 * Ovviamente l'oggetto Prolog mette a disposizione molti metodi per la 
 * configurazione, metodi che non sono presenti nell'interfaccia generele 
 * ScriptEngine. 
 * 
 * Per esempio: setTheory e getTheory, questi metodi sono un'esclusiva di 
 * tuProlog, quindi per impostare una certa teoria per lo script si ultilizza 
 * l'engine scope, impostando la keypair
 *  { PrologScriptEngine.THEORY, theory }
 * dove theory è un'istanza di Theory.
 * 
 * Prima dell'esecuzione dello script, se questo mapping è presente, si 
 * utilizza il metodo Prolog.setTheory con l'oggetto presente nell'engine 
 * scope.
 * 
 * La specifica suggerisce anche di rendere visibili allo script tutti gli 
 * oggetti presenti nell'engine scope. Per fare questo, si utilizza la 
 * JavaLibrary (se disponibile), e si invoca il metodo "register" di questa
 * su tutti gli oggetti presenti nell'engine scope, valori riservati compresi. 
 * Ovviamente se un id (cioè la chiave) dato ad un oggetto non è valido 
 * (i.e. il metodo register spara un'eccezione), la registrazione fallisce.
 * Quindi se l'applicazione host voule registrare un oggetto prima 
 * dell'esecuzione, il suo id deve essere un  atomo.
 * 
 * Per quanto riguarda l'input/output invece, ci sono dei problemi in quanto 
 * non mi pare che in tuProlog, dopo averne letto il codice sorgente, sia 
 * possibile inpostare lo standard input/output/error per gli script.
 * 
 * 
 */

/**
 * Implementation of the interface ScriptEngine for tuProlog
 * @author Andrea Bucaletti
 */
public class PrologScriptEngine implements ScriptEngine, ExceptionListener {
  
    public static final String CONTEXT = "context";
    public static final String THEORY = "theory";
    public static final String IS_SUCCESS =  "isSuccess";
    public static final String IS_HALTED = "isHalted";
    public static final String HAS_OPEN_ALTERNATIVES = "hasOpenAlternatives";
    
    protected List<Var> solveVars;
    protected String previousScript;
    protected boolean useSolveNext;
   
    protected ScriptContext defaultContext;
    protected Prolog prolog;
    
    protected SimpleIOLibrary simpleIOLib; /* Test IO */
    
    public PrologScriptEngine() {
        prolog = new Prolog();

        
        defaultContext = new PrologScriptContext(prolog);
        
        /* Test IO */
        try {
            // prolog.unloadLibrary("alice.tuprolog.lib.ISOIOLibrary");
            simpleIOLib = new SimpleIOLibrary();
            prolog.loadLibrary(simpleIOLib);
        }
        catch(InvalidLibraryException ex) {
            ex.printStackTrace();
        }
        /* Fine Test IO */        
        
        useSolveNext = false;
        previousScript = null;
        solveVars = new ArrayList<Var>();
    } 

    @Override
    public Object eval(String string) throws ScriptException {
        return eval(string, getContext());
    }

    @Override
    public Object eval(Reader reader) throws ScriptException {
        return eval(reader, getContext());
    }
    
    @Override
    public Object eval(String script, ScriptContext sc) throws ScriptException {
        /*
        As the jsr-223 part SCR.4.3.4.1.2 Script Execution :
        "In all cases, the ScriptContext used during a script execution must be
        a value in the Engine Scope of the ScriptEngine whose key is the
        String “context”.       
         */
        
        /* IO Test */
        simpleIOLib.setStandardInput(sc.getReader());
        simpleIOLib.setStandardOutput(sc.getWriter());
        /* IO Test */
        
        sc.getBindings(ScriptContext.ENGINE_SCOPE).put(CONTEXT, sc);
        return eval(script, sc.getBindings(ScriptContext.ENGINE_SCOPE));
    }

    @Override
    public Object eval(Reader reader, ScriptContext sc) throws ScriptException {
        /*
        As the jsr-223 part SCR.4.3.4.1.2 Script Execution :
        "In all cases, the ScriptContext used during a script execution must be
        a value in the Engine Scope of the ScriptEngine whose key is the
        String “context”.       
         */        

       /* IO Test */
        simpleIOLib.setStandardInput(sc.getReader());
        simpleIOLib.setStandardOutput(sc.getWriter());
        /* IO Test */        
        
        sc.getBindings(ScriptContext.ENGINE_SCOPE).put(CONTEXT, sc);
        return eval(reader, sc.getBindings(ScriptContext.ENGINE_SCOPE));
    }    
    
    @Override
    public Object eval(String script, Bindings bndngs) throws ScriptException {
        Theory theory = (Theory)bndngs.get(THEORY);
        SolveInfo info = null;
         
        /*
        As the jsr-223 part SCR.4.2.6 Bindings :
        "Each Java Script Engine has a Bindings known as its Engine Scope
        containing the mappings of script variables to their values. The
        values are often Objects in the host application. Reading the value of
        a key in the Engine Scope of a ScriptEngine returns the value of the
        corresponding script variable. Adding an Object as a value in the
        scope usually makes the object available in scripts using the specified
        key as a variable name.”.  
        
        So, all the objects in the engine scope are registered using the JavaLibrary,
        if available. This is done using the methoed 
            boolean register(Struct id, Object obj)
        of the JavaLibrary class. Any exception raised by this method will be
        forwarded, and the Object won't be registered.
         */
        
        JavaLibrary javaLibrary = (JavaLibrary) prolog.getLibrary("alice.tuprolog.lib.JavaLibrary");
        
        if(javaLibrary != null) {
            for(Map.Entry<String, Object> keyPair: bndngs.entrySet()) {
                try {
                    javaLibrary.register(new Struct(keyPair.getKey()), keyPair.getValue());
                }
                catch(InvalidObjectIdException ex) {
                    throw new ScriptException("Could not register object(" + keyPair.getKey() + "): " + ex.getMessage());
                }
            }
        }
        
        try {
            
            if(!script.equals(previousScript))
                useSolveNext = false;
            
            if(theory != null)
                prolog.setTheory(theory);
            
            if(useSolveNext)
                info = prolog.solveNext();
            else
                info = prolog.solve(script);
           
            previousScript = script;

            for(Var v : solveVars) 
                bndngs.remove(v.getName());

            solveVars.clear();

            bndngs.put(IS_SUCCESS, info.isSuccess());
            bndngs.put(IS_HALTED, info.isHalted());
            bndngs.put(HAS_OPEN_ALTERNATIVES, info.hasOpenAlternatives());
            
            if(info.isSuccess()) {
                solveVars = info.getBindingVars();
                for(Var v : solveVars)            
                    bndngs.put(v.getName(), v.getTerm().toString());             
            }
            
            useSolveNext = info.hasOpenAlternatives();
            
            return true;
        }
        catch(NoSolutionException | InvalidTheoryException | 
                MalformedGoalException | NoMoreSolutionException ex) {
            throw new ScriptException(ex);
        }
    }

    @Override
    public Object eval(Reader reader, Bindings bndngs) throws ScriptException {
        BufferedReader bReader = new BufferedReader(reader);
        String script = new String();
        try {
            while(bReader.ready()) {
                script += bReader.readLine();
            }
        }
        catch(IOException ex) {
            throw new ScriptException(ex);
        }
        return eval(script, bndngs);
    }

    @Override
    public Bindings createBindings() {
        return new PrologBindings(prolog);
    }

    @Override
    public ScriptEngineFactory getFactory() {
        return PrologScriptEngineFactory.DEFAULT_FACTORY;
    }

    @Override
    public void put(String key, Object o) {
       getBindings(ScriptContext.ENGINE_SCOPE).put(key, o);
    }

    @Override
    public Object get(String key) {
        return getBindings(ScriptContext.ENGINE_SCOPE).get(key);
    }

    @Override
    public Bindings getBindings(int i) {
        return getContext().getBindings(i);
    }

    @Override
    public void setBindings(Bindings bndngs, int i) {
        getContext().setBindings(bndngs, i);
    }

    @Override
    public ScriptContext getContext() {
        return defaultContext;
    }

    @Override
    public void setContext(ScriptContext sc) {
        defaultContext = sc;
    }

    @Override
    public void onException(ExceptionEvent ee) {
        System.out.println(ee.getMsg());
    }
    
}
