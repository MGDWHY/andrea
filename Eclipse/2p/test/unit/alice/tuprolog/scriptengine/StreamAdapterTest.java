package alice.tuprolog.scriptengine;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;

import alice.util.InputStreamAdapter;
import alice.util.OutputStreamAdapter;

public class StreamAdapterTest {
	
	public static void main(String[] args) throws IOException {
		InputStreamAdapter isa = new InputStreamAdapter(
			new BufferedReader(new InputStreamReader(System.in))
		);
		
		OutputStreamAdapter osa = new OutputStreamAdapter(
			new PrintWriter(System.out)
		);
		
		while(true) {
			int x = isa.read();
			osa.write(x);
		}
	}
}
