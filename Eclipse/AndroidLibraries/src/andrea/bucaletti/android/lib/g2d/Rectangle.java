package andrea.bucaletti.android.lib.g2d;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import andrea.bucaletti.android.lib.opengl.GLU;

public class Rectangle extends Shape2D {
	
	protected float x, y, x2, y2, width, height;
	
	protected FloatBuffer positions, texCoords;
	protected ShortBuffer indices;
	
	public Rectangle(Rectangle other) {
		this(other.x, other.y, other.width, other.height);
	}
	
	public Rectangle(float x, float y, float width, float height) {
		
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
		this.x2 = x + width;
		this.y2 = y + height;
		
		float[] v = new float[8];

		float t[] = {
			0, 1, // bottom left
			1, 1, // bottom right
			1, 0, // top right
			0, 0 // top left
		};
		
		short i[] = {
			0, 1, 2, // triangle 1
			0, 2, 3 // triangle 2
		};
		
		// bottom left
		v[0] = x;
		v[1] = y;
		
		// bottom right
		v[2] = x + width;
		v[3] = y;
		
		// top right
		v[4] = x + width;
		v[5] = y + height;
		
		// top left
		v[6] = x;
		v[7] = y + height;
		
		positions = GLU.createFloatBuffer(v);
		texCoords = GLU.createFloatBuffer(t);
		indices = GLU.createShortBuffer(i);
		
		
	}

	@Override
	public FloatBuffer getPositions() {
		return positions;
	}

	@Override
	public FloatBuffer getTexCoords() {
		return texCoords;
	}

	@Override
	public ShortBuffer getIndices() {
		return indices;
	}

	@Override
	public boolean containsPoint(float x, float y) {
		return x >= this.x && y >= this.y && x <= this.x2 && y <= this.y2;
	}
	
}
