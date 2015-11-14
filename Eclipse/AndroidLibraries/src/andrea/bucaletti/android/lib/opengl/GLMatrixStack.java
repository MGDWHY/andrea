package andrea.bucaletti.android.lib.opengl;

import andrea.bucaletti.android.lib.vecmath.Vec3f;
import android.opengl.Matrix;
/**
 * 
 * @author Andrea Bucaletti
 * A 4x4 Matrix Stack for OpenGL (column major)
 */
public class GLMatrixStack {
	
	private static final int MATRIX_LENGTH = 16;
	
	private int maxDepth;
	
	private int currentDepth;
	
	private float[][] matrices;
	
	public GLMatrixStack(int maxDepth) {
		this.maxDepth = maxDepth;
		this.currentDepth = 0;
		this.matrices = new float[maxDepth][MATRIX_LENGTH];
	}
	
	public void set(float[] matrix) {
		for(int i = 0; i < MATRIX_LENGTH; i++)
			matrices[currentDepth][i] = matrix[i];
	}
	
	public void setIdentity() {
		Matrix.setIdentityM(matrices[currentDepth], 0);
	}
	
	public void setFrustum(float left, float right, float bottom, float top, float near, float far) {
		Matrix.frustumM(matrices[currentDepth], 0, left, right, bottom, top, near, far);
	}
	
	public void setPerspective(float fovy, float aspect, float zNear, float zFar) {
		Matrix.perspectiveM(matrices[currentDepth], 0, fovy, aspect, zNear, zFar);
	}
	
	public void rotate(float angle, Vec3f axis) {
		rotate(angle, axis.x, axis.y, axis.z);
	}
	
	public void rotate(float angle, float x, float y, float z) {
		Matrix.rotateM(matrices[currentDepth], 0, angle, x, y, z);
	}
	
	public void translate(Vec3f t) {
		translate(t.x, t.y, t.z);
	}
	
	public void translate(float tx, float ty, float tz) {
		Matrix.translateM(matrices[currentDepth], 0, tx, ty, tz);
	}
	
	public void scale(float sx, float sy, float sz) {
		Matrix.scaleM(matrices[currentDepth], 0, sx, sy, sz);
	}
	
	public void multiply(float[] other) {
		float[] result = new float[MATRIX_LENGTH];
		Matrix.multiplyMM(result, 0, matrices[currentDepth], 0, other, 0);
		set(result);
	}
	
	public float[] current() {
		return matrices[currentDepth];
	}
	
	public void push() {
		if(currentDepth < maxDepth - 1) {
			// Copy the current matrix to the next spot and increment stack depth
			
			currentDepth++;
			
			set(matrices[currentDepth - 1]);
			
		} else 
			throw new IllegalArgumentException("Matrix stack max depth reached: " + currentDepth);
	}
	
	public void pop() {
		if(currentDepth > 0) {
			// decrement stack depth
			currentDepth--;
		} else
			throw new IllegalArgumentException("Matrix stack min depth reached: " + currentDepth);
	}
}
