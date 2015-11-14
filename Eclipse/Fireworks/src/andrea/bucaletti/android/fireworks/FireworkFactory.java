package andrea.bucaletti.android.fireworks;

import java.nio.ByteBuffer;

import android.graphics.Color;

public abstract class FireworkFactory {
	
	/*
	 * Returns a ByteBuffer containing all the sparks for this firework
	 */
	public abstract ByteBuffer generate(Params params);
	
	public float random() {
		return (float) Math.random();
	}
	
	public float random(float min, float max) {
		return (float)(min + Math.random() * (max - min));
	}
	
	public int random(int min, int max) {
		return (int)(min + Math.random() * (max - min));
	}
	
	public int brighterColor(int color, float brightness) {
		int r = Color.red(color);
		int g = Color.green(color);
		int b = Color.blue(color);
		return Color.rgb(
				Math.max(255, (int)(r + r * brightness)),
				Math.max(255, (int)(g + g * brightness)),
				Math.max(255, (int)(b + b * brightness))
		);		
	}
	
	public static void toFloatColor(int color, float[] result) {
		result[0] = Color.red(color) / 255.0f;
		result[1] = Color.green(color) / 255.0f;
		result[2] = Color.blue(color) / 255.0f;
	}
	
	public static class Params {
		int color0, color1;
		float x, y;
		float timePressed;
	}
}
