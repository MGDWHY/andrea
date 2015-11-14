package andrea.bucaletti.android.darts.lib;

import andrea.bucaletti.android.lib.vecmath.Vec3f;

public class Constants {
	
	// Game physics constants
	public static final float P_GRAVITY = 9.8f;
	public static final Vec3f P_GRAVITY_VECTOR = new Vec3f(0, -1, 0);
	
	public static final float P_FORCE_VELOCITY_SCALING = 0.5f; // 10 force = 5 m/s speed
	
	// Game UI constants
	
	public static final float[] UI_CROSSAIR_COLOR = {1, 0, 0, 1};
	public static final float UI_CROSSAIR_SIZE = 5;
}
