package andrea.bucaletti.android.darts.gui;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.g2d.Shape2D;


public class Crossair extends Shape2D {
	
	private FloatBuffer positions;
	
	public Crossair(float x, float y, float size) {
		float[] p = {
			x - size/2, y,
			x + size/2, y,
			x, y - size/2,
			x, y + size/2
		};
		
		positions = GLU.createFloatBuffer(p);
	}

	@Override
	public FloatBuffer getPositions() {
		return positions;
	}

	@Override
	public FloatBuffer getTexCoords() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ShortBuffer getIndices() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean containsPoint(float x, float y) {
		return false;
	}
	
}
