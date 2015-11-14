package andrea.bucaletti.android.lib.graphics;

import java.util.ArrayList;
import java.util.List;
import android.graphics.Paint;

public class DrawUtil {
	public static List<String> breakTextWords(Paint paint, String text, float maxWidth) {
		
		ArrayList<String> result = new ArrayList<String>();
		String[] lines = text.split("\\r?\\n");
		String currentLine = new String();
		
		for(int i = 0; i < lines.length; i++) {
			
			String[] words = lines[i].split("\\s+");
			
			for(int j = 0 ; j < words.length; j++) {
			
				float wlen = paint.measureText(words[j]);
				float cllen = paint.measureText(currentLine);
				
				if(wlen + cllen > maxWidth) {
					result.add(currentLine);
					currentLine = words[j] + " ";
				} else 
					currentLine += words[j] + " ";
			}
			
			if(currentLine.length() > 0) {
				result.add(currentLine + "\n");
				currentLine = new String();
			}
		}
		
		return result;
		
	}
}
