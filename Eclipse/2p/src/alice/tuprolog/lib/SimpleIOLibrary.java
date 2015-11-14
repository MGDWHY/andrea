/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package alice.tuprolog.lib;

import alice.tuprolog.Int;
import alice.tuprolog.Library;
import alice.tuprolog.Operator;
import alice.tuprolog.PrologError;
import alice.tuprolog.Struct;
import alice.tuprolog.Term;
import alice.tuprolog.Var;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;

/**
 *
 * @author Andrea
 */
public class SimpleIOLibrary extends Library {
    
    private static final String stdInputAlias = "user_input";
    private static final String stdOutputAlias = "user_output";
    
    private HashMap<String, StreamWrapper> streams;
    private HashMap<String, StreamWrapper> streamsByAlias;
    
    private String stdInputName, stdOutputName;
    
    private StreamWrapper currentInput, currentOutput;
    
    public SimpleIOLibrary() {
        super();
        
        streams = new HashMap<>();
        streamsByAlias = new HashMap<>();
        
        currentInput = new StreamWrapper(new InputStreamReader(System.in));
        currentOutput = new StreamWrapper(new PrintWriter(System.out));
        
        stdInputName = currentInput.toString();
        stdOutputName = currentOutput.toString();
        
        streams.put(stdInputName, currentInput);
        streams.put(stdOutputName, currentOutput);
        
        streamsByAlias.put(stdInputAlias, currentInput);
        streamsByAlias.put(stdOutputAlias, currentOutput);
    }
    
    public void setStandardInput(InputStream is) { setStandardInput(new StreamWrapper(is));  } 
    
    public void setStandardInput(Reader rd) { setStandardInput(new StreamWrapper(rd)); }
    
    public void setStandardOutput(OutputStream os) { setStandardOutput(new StreamWrapper(os)); }
    
    public void setStandardOutput(Writer wr) { setStandardOutput(new StreamWrapper(wr)); }
    
    /* Temporary library functions */
    
    public boolean print_active_streams_0() throws PrologError {
        
        Writer stdOut = findStream(new Struct(stdOutputAlias)).getWriter();
        
        try { 

            for(Map.Entry<String, StreamWrapper> entry : streams.entrySet())
                stdOut.write(entry.getKey() + " -> " + entry.getValue().asString() + "\n");

            for(Map.Entry<String, StreamWrapper> entry : streamsByAlias.entrySet())
                stdOut.write(entry.getKey() + " -> " + entry.getValue().asString() + "\n");
        }
        catch(IOException ex) {
            throw PrologError.system_error(new Struct(ex.getMessage()));
        }
     
        
        return true;
    }
    
    /* Library functions */
    
    public boolean close_1(Term stream_or_alias)throws PrologError{

        StreamWrapper sw = findStream(stream_or_alias);
        
        if(!sw.toString().equals(stdInputName) && !sw.toString().equals(stdOutputName)) {
            try { sw.closeStream(); }
            
            catch(IOException ex) {
                throw PrologError.system_error(new Struct("An error has occurred on stream closure."));
            }
            
            streams.remove(sw.toString());
            
            Struct aliases = (Struct)sw.get("alias");
            
            for(int k = 0; k < aliases.getArity(); k++) {
                String alias = ((Struct)aliases.getArg(k).getTerm()).getName();
                streamsByAlias.remove(alias);
            }
            
        } 
        
        return true;
            
    }    
    
    public boolean open_3(Term source_sink, Term mode, Term stream)throws PrologError{
   
        source_sink = source_sink.getTerm();
        File file = new File(((Struct)source_sink).getName());
        if(!file.exists()){
            throw PrologError.existence_error(engine.getEngineManager(), 1, "source_sink", source_sink, new Struct("File not found"));
        }
        mode = mode.getTerm();
        if (source_sink instanceof Var) {
            throw PrologError.instantiation_error(engine.getEngineManager(), 1);
        }

        if(mode instanceof Var){
            throw PrologError.instantiation_error(engine.getEngineManager(), 2);
        }
        else if(!mode.isAtom()){
            throw PrologError.type_error(engine.getEngineManager(), 1, "atom",mode);
        }
        
        if(!(stream instanceof Var)){
            throw PrologError.type_error(engine.getEngineManager(), 3, "variable", stream);
        }
        
        //siccome ? una open con la lista delle opzioni vuota, inizializzo comunque le opzioni
        //e inoltre inserisco i valori che gi? conosco come file_name,mode,input,output e type.
        
        StreamWrapper sw = new StreamWrapper();
        
        Struct structMode = (Struct)mode;
        
        if(true){
            Struct in_out = (Struct)source_sink;
            Struct value = new Struct(in_out.getName());
            String type = ((Struct)sw.get("type")).getName();
            sw.set("file_name", value);
            sw.set("mode", mode);
                
            if(structMode.getName().equals("write")){
                try{
                    sw.setStream(new FileWriter(file));
                }
                catch(Exception e){
                    //potrebbe essere sia FileNotFoundException sia SecurityException
                    throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                            new Struct("The source_sink specified by Source_sink cannot be opened."));
                }                 
                
                streams.put(sw.toString(), sw);
                
                return unify(stream, new Struct(sw.toString()));
            }
            else if(structMode.getName().equals("read")){
                    try {          
                        sw.setStream(new FileReader(file));
                    } catch (Exception e) {
                        throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                                new Struct("The source_sink specified by Source_sink cannot be opened."));
                    }
                                 
                    streams.put(sw.toString(), sw);
                    return unify(stream, new Struct(sw.toString()));
            }
            else if(structMode.getName().equals("append")){
                try{
                    sw.setStream(new FileWriter(file, true));
                }
                catch(Exception e){
                    throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                            new Struct("The source_sink specified by Source_sink cannot be opened."));
                }
                   
                streams.put(sw.toString(), sw);
                return unify(stream, new Struct(sw.toString()));
            }
            else{
                throw PrologError.domain_error(engine.getEngineManager(), 1,"stream", in_out);    
            }
        }
        else{
            PrologError.system_error(new Struct("A problem has occurred with the initialization of the hashmap properties."));
            return false;
        }
    }    
    
    public boolean open_4(Term source_sink, Term mode, Term stream, Term options)throws PrologError
    {

        source_sink = source_sink.getTerm();
        mode = mode.getTerm();
        
        if (source_sink instanceof Var) {
            throw PrologError.instantiation_error(engine.getEngineManager(), 1);
        }
        
        File file = new File(((Struct)source_sink).getName());
        if(!file.exists()){
            throw PrologError.existence_error(engine.getEngineManager(), 1, "source_sink", source_sink, new Struct("File not found."));
        }

        if(mode instanceof Var){
            throw PrologError.instantiation_error(engine.getEngineManager(), 2);
        }
        else if(!mode.isAtom()){
            throw PrologError.type_error(engine.getEngineManager(), 1, "atom",mode);
        }
        
        if(!(stream instanceof Var)){
            throw PrologError.type_error(engine.getEngineManager(), 3, "variable", stream);
        }
        
        StreamWrapper sw = new StreamWrapper();
                 
        if(true){
            Struct openOptions = (Struct)options;
            Struct in_out = (Struct)source_sink;
            if(openOptions.isList()){
                if(!openOptions.isEmptyList()){
                    Iterator<? extends Term> i = openOptions.listIterator();
                    while(i.hasNext()){
                        Struct option = null;
                        Object obj = i.next();
                        if(obj instanceof Var){
                            throw PrologError.instantiation_error(engine.getEngineManager(), 4);
                        }
                        option = (Struct)obj;
                        if(!sw.isProperty(option.getName())){
                            throw PrologError.domain_error(engine.getEngineManager(), 4, "stream_option", option);
                        }
                        
                        if(option.getName().equals("alias")) {
                            for(int k = 0; k < option.getArity(); k++)
                                if(streamExists(option.getArg(k)))
                                    throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", 
                                            option.getArg(k), new Struct("Alias is already associated with an open stream."));
                            
                            for(int k = 0; k < option.getArity(); k++) {
                                String alias = ((Struct)option.getArg(k).getTerm()).getName();
                                streamsByAlias.put(alias, sw);
                            }
 
                            int arity = option.getArity();
                            Term[] arrayTerm = new Term[arity];
                            for(int k = 0; k<arity; k++){
                                arrayTerm[k] = option.getArg(k);
                            }
                            sw.set(option.getName(),new Struct(".",arrayTerm));

                        } else{
                            Struct value = null;
                            value =(Struct) option.getArg(0);
                            sw.set(option.getName(),value);
                        }
                    }
                    sw.set("mode", mode);
                    sw.set("file_name", source_sink);
                }
            }
            else{
                throw PrologError.type_error(engine.getEngineManager(), 4, "list", openOptions);
            }
            
            Struct structMode = (Struct)mode;
            Struct structType = (Struct)sw.get("type");
     
            
            if(structMode.getName().equals("write")){
                try{
                    
                    if(structType.getName().equals("text"))
                        sw.setStream(new FileWriter(in_out.getName()));
                    else if(structType.getName().equals("binary"))
                        sw.setStream(new FileOutputStream(in_out.getName()));

                }
                catch(Exception e){
                    //potrebbe essere sia FileNotFoundException sia SecurityException
                    throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                            new Struct("The source_sink specified by Source_sink cannot be opened."));
                }

                streams.put(sw.toString(), sw);
                
                return unify(stream, new Struct(sw.toString()));
            }
            else if(structMode.getName().equals("read")){
                
                    FileInputStream fis = null;
                    String reposition = ((Struct)sw.get("reposition")).getName();
                    
                    try {
                        
                        fis = new FileInputStream(in_out.getName());
                        
                        if(structType.getName().equals("text")) {
                            InputStreamReader rd = new InputStreamReader(fis);
                            if(reposition.equals("true"))                          
                                rd.mark(fis.available() + 5);
                            sw.setStream(new InputStreamReader(fis));
                        } else {
                            sw.setStream(fis);                        
                            if(reposition.equals("true")) {
                                fis.mark(fis.available() + 5);
                            }
                        }
                    } catch (Exception e) {
                        throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                                new Struct("The source_sink specified by Source_sink cannot be opened."));
                    }
                    
                    streams.put(sw.toString(), sw);
                    
                    return unify(stream, new Struct(sw.toString()));
            }
            else if(structMode.getName().equals("append")){
                try{
                    if(structType.getName().equals("text"))
                        sw.setStream(new FileWriter(in_out.getName(), true));
                    else
                        sw.setStream(new FileOutputStream(in_out.getName(), true));
                }
                catch(Exception e){
                    throw PrologError.permission_error(engine.getEngineManager(), "open", "source_sink", source_sink,
                            new Struct("The source_sink specified by Source_sink cannot be opened."));
                }
             
                streams.put(sw.toString(), sw);
                
                return unify(stream, new Struct(sw.toString()));
            }
            else{
                throw PrologError.domain_error(engine.getEngineManager(),2,"io_mode", mode);    
            }
        }
        else{
            PrologError.system_error(new Struct("A problem has occurred with initialization of properties' hashmap."));
            return false;
        }
    }    
    
    public boolean read_2(Term stream_or_alias, Term in_term)throws PrologError{
        Struct options = new Struct(".",new Struct());
        return read_term_3(stream_or_alias,in_term,options);
    }    
    
    public boolean read_term_2(Term in_term, Term options)throws PrologError {
        Struct stream_or_alias = new Struct(currentInput.toString());
        return read_term_3(stream_or_alias,in_term,options);
    }
    
    public boolean read_term_3(Term stream_or_alias, Term in_term, Term options) throws PrologError{
        
        Reader reader = null;
        StreamWrapper sw = findStream(stream_or_alias);
                
        if(options instanceof Var){
            throw PrologError.instantiation_error(engine.getEngineManager(), 3); 
        }
        
        try { reader = sw.getReader(); }
        catch(ClassCastException ex) {
            throw PrologError.permission_error(engine.getEngineManager(), "input", "binary_stream", 
                    stream_or_alias, new Struct("Target stream is associated with a binary stream."));
        }
        
        Struct variables = null;
        Struct variable_names = null;
        Struct singletons = null;
        
        boolean variables_bool=false;
        boolean variable_names_bool=false;
        boolean singletons_bool=false;
        
        Struct readOptions = (Struct)options; 
        if(readOptions.isList()){
            if(!readOptions.isEmptyList()){
                Iterator<? extends Term> i = readOptions.listIterator();
                while(i.hasNext()){
                    Struct option = null;
                    Object obj = i.next();
                    if(obj instanceof Var){
                        throw PrologError.instantiation_error(engine.getEngineManager(), 3);
                    }                    
                    option = (Struct)obj;
                    if(option.getName().equals("variables")){
                        variables_bool=true;
                    }
                    else if(option.getName().equals("variable_name")){
                        variable_names_bool=true;
                    }
                    else if(option.getName().equals("singletons")){
                        singletons_bool=true;
                    }
                    else{
                        PrologError.domain_error(engine.getEngineManager(), 3, "read_option", option);
                    }
                }
            }
        }
        else{
            throw PrologError.type_error(engine.getEngineManager(), 3, "list", options);
        }
        
        try {
            int ch = 0;
            
            boolean open_apices = false;
            boolean open_apices2 = false;
            
            in_term = in_term.getTerm();
            StringWriter strWriter = new StringWriter();
            String st = "";
            do {
                ch = reader.read();
                
                if (ch == -1) {
                    break;
                }
                boolean can_add = true;
                
                if (ch=='\''){
                    if (!open_apices){
                        open_apices = true;
                    } else {
                        open_apices = false;
                    }
                } else if (ch=='\"'){
                    if (!open_apices2){
                        open_apices2 = true;
                    } else {
                        open_apices2 = false;
                    }
                } else {
                    if (ch=='.'){
                        if (!open_apices && !open_apices2){
                            break;
                        }
                    }
                }
                if (can_add){
                    strWriter.append((char)ch);
                }
                
                
            } while (true);
            
            st = strWriter.toString();
                                    
            if(variables_bool == false && variable_names_bool == false && singletons_bool == false){
                return unify(in_term, getEngine().toTerm(st));
            }
            Var input_term = new Var();
            unify(input_term,Term.createTerm(st));
                    
            //opzione variables + variables_name
            List<Term> variables_list = new ArrayList<Term>();
            analizeTerm(variables_list,input_term);
            
            HashMap<Term,String> associations_table = new HashMap<Term,String>(variables_list.size());

            //la hashtable sottostante la costruisco per avere le associazioni 
            //con le variabili '_' Queste infatti non andrebbero inserite all'interno della
            //read_option variable_name, ma vanno sostituite comunque da variabili nel termine letto.
            HashMap<Term,String> association_for_replace = new HashMap<Term,String>(variables_list.size());
            
            LinkedHashSet<Term> set = new LinkedHashSet<Term>(variables_list);
            List<Var> vars = new ArrayList<Var>();

            if(variables_bool == true){
                int num = 0;
                for(Term t:set){
                    num++;
                    if(variable_names_bool == true){
                        association_for_replace.put(t, "X"+num);
                        if(!((t.toString()).startsWith("_"))){
                            associations_table.put(t, "X"+num);
                        }
                    }
                    vars.add(new Var("X"+num));
                }
            }
                        
            //opzione singletons
            List<Term> singl = new ArrayList<Term>();
            int flag = 0;
            if(singletons_bool == true){
                List<Term> temporanyList = new ArrayList<Term>(variables_list);
                for(Term t:variables_list){
                    temporanyList.remove(t);
                    flag = 0;
                    for(Term temp:temporanyList){
                        if(temp.equals(t)){
                            flag = 1;
                        }
                    }
                    if(flag == 0){
                        if(!((t.toString()).startsWith("_"))){
                            singl.add(t);
                        }
                    }
                    temporanyList.add(t);
                }
            }
                        
            //unisco le liste con i relativi termini
            Iterator<? extends Term> i = readOptions.listIterator();
            Struct option = null;
            while(i.hasNext()){
                Object obj = i.next();
                option = (Struct)obj;
                if(option.getName().equals("variables")){
                    variables = new Struct();
                    variables = (Struct) Term.createTerm(vars.toString());
                    unify(option.getArg(0),variables);
                }
                else if(option.getName().equals("variable_name")){
                    variable_names = new Struct();
                    variable_names = (Struct)Term.createTerm(associations_table.toString());
                    unify(option.getArg(0),variable_names);
                }
                else if(option.getName().equals("singletons")){
                    singletons = new Struct();
                    singletons = (Struct)Term.createTerm(singl.toString());
                    unify(option.getArg(0),singletons);
                }
            }
            
            String string_term = input_term.toString();
            
            for(Map.Entry<Term,String> entry:association_for_replace.entrySet()){
                String regex = entry.getKey().toString();
                String replacement = entry.getValue();
                string_term = string_term.replaceAll(regex, replacement);
            }
            
            return unify(in_term, getEngine().toTerm(string_term));
        } catch (Exception ex){
            return false;
        }
    }    
    
    public boolean flush_output_0()throws PrologError {    
        try {
            if(currentOutput.isBinary())
                currentOutput.getOutputStream().flush();
            else
                currentOutput.getWriter().flush();
        } catch (IOException e) {
            throw PrologError.system_error(new Struct("An error has occurred in method 'flush_output_0'."));
        }
        return true;
    }    
    
    public boolean write_1(Term out_term) throws PrologError{
        return write_iso_1(out_term);
    }
    
    public boolean write_iso_1(Term out_term) throws PrologError{

        Struct stream_or_alias = new Struct(currentOutput.toString());
        Struct options = new Struct(".",new Struct("quoted",new Struct("false")),
                new Struct(".",new Struct("ignore_ops",new Struct("false")),
                new Struct(".",new Struct("numbervars",new Struct("true")),new Struct())));
        return write_term_3(stream_or_alias,out_term,options);
    }    
    
    public boolean write_term_3(Term stream_or_alias, Term out_term, Term optionsTerm)throws PrologError{

        out_term = out_term.getTerm();
                
        StreamWrapper sw = findStream(stream_or_alias);
        Struct writeOptionsList = (Struct)optionsTerm.getTerm(); 
        
        boolean quoted = false;
        boolean ignore_ops = false;
        boolean numbervars = false;
        Struct writeOption = null;
        Writer writer = null;
        
        try  { writer = sw.getWriter(); }
        catch(ClassCastException ex) {
            throw PrologError.permission_error(engine.getEngineManager(), "output", "binary_stream", stream_or_alias, 
                    new Struct("Target stream is associated with a binary stream."));
        }
        
        if(writeOptionsList.isList()){
            if(!writeOptionsList.isEmptyList()){
                Iterator<? extends Term> i = writeOptionsList.listIterator();
                while(i.hasNext()){
                    //siccome queste opzioni sono true o false analizzo direttamente il loro valore
                    //e restituisco il loro valore all'interno dell'opzione corrispondente
                    Object obj = i.next();
                    if(obj instanceof Var){
                        throw PrologError.instantiation_error(engine.getEngineManager(), 3);
                    }
                    writeOption = (Struct)obj;
                    if(writeOption.getName().equals("quoted")){
                        quoted = ((Struct) writeOption.getArg(0)).getName().equals("true")? true:false;
                    }
                    else if(writeOption.getName().equals("ignore_ops")){
                        ignore_ops =((Struct) writeOption.getArg(0)).getName().equals("true")? true:false;
                    }
                    else if(writeOption.getName().equals("numbervars")){
                        numbervars = ((Struct) writeOption.getArg(0)).getName().equals("true")? true:false;
                    }
                    else{
                        throw PrologError.domain_error(engine.getEngineManager(), 3, "write_options", writeOptionsList.getTerm());
                    }
                }
            }
        }
        else{
            PrologError.type_error(engine.getEngineManager(), 3, "list", writeOptionsList);
        }
        try{
            if(!out_term.isCompound() && !(out_term instanceof Var)){
                
                if(quoted == true)
                    writer.write(alice.util.Tools.removeApices(out_term.toString()));
                else
                    writer.write(out_term.toString());
                
                return true;
            }
            
            
            if(out_term instanceof Var) {
                
                if(quoted == true)
                    writer.write(alice.util.Tools.removeApices(out_term.toString()) + " ");
                else
                    writer.write(out_term.toString() + " ");

                return true;
            }
            
            Struct term = (Struct)out_term;
            String result = "";
            HashMap<String,Boolean> options = new HashMap<String,Boolean>(3);
            options.put("numbervars", numbervars);
            options.put("ignore_ops", ignore_ops);
            options.put("quoted", quoted);
            
            result = createString(options,term);
            
            writer.write(result);
            
        }
        catch(IOException ioe){
            PrologError.system_error(new Struct("Write error has occurred in write_term/3."));
        }
        return true;
    }    
    
    /* Helper functions */
    
    private void analizeTerm(List<Term> variables,Term t){
        if(!t.isCompound()){
            variables.add(t);
        }
        else{
            Struct term_struct = (Struct)t.getTerm();
            for(int i = 0;i<term_struct.getArity();i++){
                analizeTerm(variables,term_struct.getArg(i));
            }
        }
    }    
    
    private String createString(HashMap<String,Boolean> options, Struct term){
        
        boolean numbervars = options.get("numbervars");
        boolean quoted = options.get("quoted");
        boolean ignore_ops = options.get("ignore_ops");
        
        String result = "";
        String list = "";
        if(term.isList()){
            list = printList(term,options);
            if(ignore_ops==false)
                return "[" + list +"]";
            else
                return list;
        }
                
        List<Operator> operatorList = engine.getCurrentOperatorList();
        String operator = "";
        int flagOp = 0;
        for(Operator op : operatorList){
            if(op.name.equals(term.getName())){
                operator = op.name;
                flagOp = 1;
                break;
            }
        }
        
        if(flagOp == 0){
            result+=term.getName()+"(";
        }
        
        int arity = term.getArity();
        for(int i = 0; i<arity; i++){
            if(i > 0 && flagOp==0)
                result += ",";
            Term arg = term.getArg(i);
            if(arg instanceof alice.tuprolog.Number){
                if(term.getName().contains("$VAR")){
                //sono nel tipo $VAR
                    if(numbervars == true){
                        Int argNumber = (Int)term.getArg(i);
                        int res = argNumber.intValue() % 26;
                        int div = argNumber.intValue()/26;
                        Character ch = 'A';
                        int num = (ch+res);
                        result = new String(Character.toChars(num));
                        if(div != 0){
                            result += div;
                        }
                    }
                    else{
                        if(quoted == true){
                            return term.toString();
                        }
                        else{
                            result += alice.util.Tools.removeApices(arg.toString()); 
                        }
                    }
                    continue;
                }
                else{
                //e' un numero da solo o un operando
                    if(ignore_ops == false){
                        result += arg.toString();
                        if(i%2 == 0 && operator != ""){
                            result +=" "+operator+" ";
                        }
                        continue;
                    }
                    else{
                        result = term.toString();
                        return result;
                    }
                }
            }
            else if(arg instanceof Var){
            // stampo il toString della variabile
                if(ignore_ops == false){
                    result+= arg.toString();
                    if(i%2 == 0 && operator != ""){
                        result +=" "+operator+" ";
                    }
                    continue;
                }
                else{
                    result+= arg.toString();
                }
                continue;
            }
            else if(arg.isCompound()){
                if(ignore_ops == false){
                    result+= createString(options,(Struct)arg);
                    if(i%2 == 0 && operator != ""){
                        result +=" "+operator+" ";
                    }
                    continue;
                }
                else{
                    result+= createString(options,(Struct)arg);
                }
                
            }
            else{
                if(quoted == true){
                    if(ignore_ops == false){
                        result += arg.toString();
                        if(i%2 == 0 && operator != ""){
                            result +=" "+operator+" ";
                        }
                        continue;
                    }
                    else{
                        result += arg.toString();
                    }
                }
                    
                else{
                    if(ignore_ops == false){
                        result += alice.util.Tools.removeApices(arg.toString());
                        if(i%2 == 0 && operator != ""){
                            result +=" "+operator+" ";
                        }
                        continue;
                    }
                    else{
                        result += alice.util.Tools.removeApices(arg.toString());
                    }
                }
            }
        }
        
        if(flagOp == 0 && result.contains("(")){
            result += ")";
        }
        return result;        
    }
  
    private String printList(Struct term, HashMap<String, Boolean> options) {
        
        //boolean numbervars = options.get("numbervars");
        //boolean quoted = options.get("quoted");
        boolean ignore_ops = options.get("ignore_ops");
        
        String result = "";
        
        if(ignore_ops == true){
            result="'"+term.getName()+"'"+" (";
            for(int i = 0; i<term.getArity(); i++){
                if(i > 0){
                    result+=",";
                }
                if(term.getArg(i).isList() && !(term.getArg(i).isEmptyList())){
                    result += printList((Struct)term.getArg(i),options);
                }
                else{
                    result += term.getArg(i);
                }
            }
            return result + ")";
        }
        else{
            for(int i = 0; i<term.getArity(); i++){
                if(i > 0 && !(term.getArg(i).isEmptyList())){
                    result+=",";
                }
                if((term.getArg(i)).isCompound() && !(term.getArg(i).isList())){
                    result += createString(options,(Struct)term.getArg(i));
                }
                else{
                    //costruito cosi' per un problema di rappresentazione delle []
                    if((term.getArg(i).isList()) && !(term.getArg(i).isEmptyList()))
                        result+=printList((Struct)term.getArg(i),options);
                    else{
                        if(!(term.getArg(i).isEmptyList()))
                            result+= term.getArg(i).toString();
                    }
                        
                }
            }
            return result ;
        }
    }    
    
    public boolean streamExists(Term streamOrAlias) throws PrologError {
        streamOrAlias = streamOrAlias.getTerm();
        
        if (streamOrAlias instanceof Var) { //controlla che non sia una variabile
            throw PrologError.instantiation_error(engine.getEngineManager(), 1);
        }     
        
        String streamName = ((Struct)streamOrAlias).getName();
        
        if(!streams.containsKey(streamName)) {
            return streamsByAlias.containsKey(streamName);
        }else return true;
        
    }
    
    private StreamWrapper findStream(Term streamOrAlias) throws PrologError {
        
        streamOrAlias = streamOrAlias.getTerm();
        
        if (streamOrAlias instanceof Var) { //controlla che non sia una variabile
            throw PrologError.instantiation_error(engine.getEngineManager(), 1);
        }     
        
        String streamName = ((Struct)streamOrAlias).getName();
        
        StreamWrapper result = streams.get(streamName);
        
        if(result == null) {
            result = streamsByAlias.get(streamName);
            if(result == null)
                throw PrologError.existence_error(engine.getEngineManager(), 1, "stream", streamOrAlias, new Struct("Input stream should be opened."));
        }
        
        return result;
    }
    
    private void setStandardInput(StreamWrapper sw) {
        if(currentInput.toString().equals(stdInputName))
            currentInput = sw;
        
        streams.remove(stdInputName);
        
        stdInputName = sw.toString();

        streams.put(stdInputName, sw);     
        streamsByAlias.put(stdInputAlias, sw);
    }
    
    private void setStandardOutput(StreamWrapper sw) {
        if(currentOutput.toString().equals(stdOutputName))
            currentOutput = sw;
        
        streams.remove(stdOutputName);
        
        stdOutputName = sw.toString();
        
        streams.put(stdOutputName, sw);         
        streamsByAlias.put(stdOutputAlias, sw);
    }    
    
    private class StreamWrapper {
        
        private Object stream;
        private HashMap<String, Term> properties;

        private boolean isInput;
        private boolean isBinary;        
        
        public StreamWrapper() {
            
            properties = new HashMap<>();
            
            Struct s = new Struct();
            properties.put("file_name", s);
            properties.put("mode", s);
            properties.put("input", new Struct("false"));
            properties.put("output", new Struct("false"));
            properties.put("alias", s);
            properties.put("position", new Int(0));
            properties.put("end_of_stream", new Struct("not"));
            properties.put("eof_action", new Struct("error"));
            properties.put("reposition", new Struct("false"));
            properties.put("type", new Struct("text"));            
        }
        
        public StreamWrapper(InputStream is) { 
            this();
            setStream(is);
        }
        
        public StreamWrapper(OutputStream os) { 
            this();
            setStream(os);
        }
        
        public StreamWrapper(Reader rd) { 
            this();
            setStream(rd);
        }
        
        public StreamWrapper(Writer wr) { 
            this();
            setStream(wr);
        }
        
        public boolean isProperty(String key) {
            return properties.containsKey(key);
        }
        
        public Term set(String key, Term value) {
            if(!properties.containsKey(key))
                return null;
            
            return properties.put(key, value);
        }
        
        public Term get(String key) {
            return properties.get(key);
        }
        
        public void setStream(InputStream is) { setStream(true, true, is); }
        public void setStream(OutputStream os) { setStream(false, true, os); }
        public void setStream(Reader rd) { setStream(true, false, rd); }
        public void setStream(Writer wr) { setStream(false, false, wr); }
        
        private void setStream(boolean isInput, boolean isBinary, Object stream) {
            
            this.isInput = isInput;
            this.isBinary = isBinary;
            this.stream = stream;
            
            set("type", new Struct(isBinary ? "binary" : "text"));
            set(isInput ? "input" : "output", new Struct("true"));
                
        }
        
        public InputStream getInputStream() { return (InputStream) stream; }
        public OutputStream getOutputStream() { return (OutputStream) stream; }
        public Reader getReader() { return (Reader) stream; }
        public Writer getWriter() { return (Writer) stream; }
        
        public boolean isBinary() { return isBinary; }
        public boolean isText() { return !isBinary; }
        
        public boolean isInput() { return isInput; }
        public boolean isOutput() { return !isInput; }
        
        public void closeStream() throws IOException {
   
            if(!isInput)
                if(isBinary)
                    getOutputStream().flush();
                else
                    getWriter().flush();
            
            ((Closeable)stream).close();
        }
        
        public String asString() {
            
            return "[" + stream + ", " + (isInput ? "input" : "output") + ", " + (isBinary ? "binary" : "text")
                + ", alias(" + get("alias").toString() + ")]";    
        }
    }
}
