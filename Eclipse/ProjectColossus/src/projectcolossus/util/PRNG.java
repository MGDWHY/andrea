package projectcolossus.util;

import java.util.Random;

import andrea.bucaletti.android.lib.vecmath.Vec3f;

public class PRNG {
	
	private static Random random;
	
	static {
		random = new Random();
	}
	
	public static int nextInt() {
		return random.nextInt();
	}
	
	public static float nextFloat(float max) {
		return random.nextFloat() * max;
	}
	
	public static float nextFloat(float min, float max) {
		return random.nextFloat() * (max - min) + min;
	}
	
	public static float nextAngleRadians() {
		return random.nextFloat() * (float)(Math.PI * 2);
	}
	
	public static Vec3f randomDirection() {
		return new Vec3f(
			nextFloat(-1, 1),
			nextFloat(-1, 1),
			nextFloat(-1, 1)
		).normalize();
	}
}
