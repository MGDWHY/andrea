package andrea.bucaletti.android.lib;

import android.util.Log;


public class Timer {
	
	private long startTime;
	private long prevTime;
	
	public Timer() {
		reset();
	}
	
	public float dt() {
		long now = System.currentTimeMillis();
		float dt = (now - prevTime) / 1000.0f;
		prevTime = now;
		return dt;
	}
	
	public float time() {
		return (System.currentTimeMillis() - startTime) / 1000.0f;
	}
	
	public void reset() {
		startTime = prevTime = System.currentTimeMillis();
	}
}
