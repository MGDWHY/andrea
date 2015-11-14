package andrea.bucaletti.android.lib.vecmath;

import java.util.Comparator;

public class Vec4f {
	private float[] v;
	
	public static final Comparator<Vec4f> ZComparator;
	
	static {
		ZComparator = new ZComparatorImpl();
	}	
	
	public Vec4f() {
		this(0, 0, 0, 0);
	}
	
	public Vec4f(Vec3f xyz, float w) {
		this(xyz.x, xyz.y, xyz.z, w);
	}
	
	public Vec4f(float x, float y, float z, float w) {
		v = new float[4];
		this.v[0] = x;
		this.v[1] = y;
		this.v[2] = z;
		this.v[3] = w;
	}
	
	public Vec4f add(Vec4f other) {
		return new Vec4f(
				v[0] + other.v[0],
				v[1] + other.v[1],
				v[2] + other.v[2],
				v[3] + other.v[3]
		);
	}
	
	public Vec4f scale(float k) {
		return new Vec4f(
				v[0] * k,
				v[1] * k,
				v[2] * k,
				v[3] * k
		);		
	}
	
	public Vec3f toEuclideanVector() {
		return new Vec3f(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
	}
	
	public float[] getComponents() {
		return v;
	}
	
	public void set(Vec4f other) {
		for(int i = 0; i < 4; i++)
			this.v[i] = other.v[i];
	}
	
	public void setComponent(int index, float value) {this.v[index] = value;}
	public float getComponent(int index) { return this.v[index]; }
	
	private static class ZComparatorImpl implements Comparator<Vec4f> {

		@Override
		public int compare(Vec4f lhs, Vec4f rhs) {
			if(lhs.v[2] < rhs.v[2])
				return -1;
			else if(lhs.v[2] > rhs.v[2])
				return 1;
			else
				return 0;
		}
		
	}	
	
}
