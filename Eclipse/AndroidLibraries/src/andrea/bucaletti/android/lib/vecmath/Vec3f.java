package andrea.bucaletti.android.lib.vecmath;

import java.util.Comparator;

public class Vec3f {
	
	public static final Comparator<Vec3f> ZComparator;
	
	static {
		ZComparator = new ZComparatorImpl();
	}
	
	public float x, y, z;
	
	public Vec3f() {
		this(0, 0, 0);
	}
	
	public Vec3f(float x, float y, float z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public Vec3f(Vec3f other) {
		this(other.x, other.y, other.z);
	}
	
	public void set(float x, float y, float z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public void set(Vec3f other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
	
	public Vec3f add(Vec3f other) {
		return new Vec3f(x + other.x, y + other.y, z + other.z);
	}
	
	public Vec3f sub(Vec3f other) {
		return new Vec3f(x - other.x, y - other.y, z - other.z);	
	}
	
	public Vec3f scale(float k) {
		return new Vec3f(k * x, k * y, k * z);
	}
	
	public Vec3f normalize() {
		float len = length();
		
		return new Vec3f(x/len, y/len, z/len);	
	}
	
	public float dot(Vec3f other) {
		return x * other.x + y * other.y + z * other.z;
	}
	
	public Vec3f cross(Vec3f other) {
		float sx = y * other.z - z * other.y;
		float sy = z * other.x - x * other.z;
		float sz = x * other.y - y * other.x;
		
		return new Vec3f(sx, sy, sz);
	}
	
	public float length() {
		return (float)Math.sqrt(x*x + y*y + z*z);
	}
	
	public float[] toArray() {
		return new float[] { x, y, z};
	}
	
	@Override
	public String toString() {
		return "(" + x + ", " + y + ", " + z + ")";
	}
	
	private static class ZComparatorImpl implements Comparator<Vec3f> {

		@Override
		public int compare(Vec3f lhs, Vec3f rhs) {
			if(lhs.z < rhs.z)
				return -1;
			else if(lhs.z > rhs.z)
				return 1;
			else
				return 0;
		}
		
	}
	
}
