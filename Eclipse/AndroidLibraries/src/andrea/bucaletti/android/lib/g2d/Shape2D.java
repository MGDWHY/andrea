package andrea.bucaletti.android.lib.g2d;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

public abstract class Shape2D {
	
	public abstract FloatBuffer getPositions();
	public abstract FloatBuffer getTexCoords();
	public abstract ShortBuffer getIndices();
	
	public abstract boolean containsPoint(float x, float y);
	
	
	public void drawIndexed(int positionHandle, int texCoordHandle) {
		drawIndexed(positionHandle, texCoordHandle, GLES20.GL_TRIANGLES);
	}
	
	public void drawIndexed(int positionHandle, int texCoordHandle, int mode) {
		if(positionHandle >= 0) {
			GLES20.glEnableVertexAttribArray(positionHandle);
			GLES20.glVertexAttribPointer(positionHandle, 2, GLES20.GL_FLOAT, false, 0, getPositions());
		}
		
		if(texCoordHandle >= 0) {
			GLES20.glEnableVertexAttribArray(texCoordHandle);
			GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 0, getTexCoords());
		}
		
		GLES20.glDrawElements(mode, getIndices().capacity(), GLES20.GL_UNSIGNED_SHORT, getIndices());
		
		if(positionHandle >= 0)
			GLES20.glDisableVertexAttribArray(positionHandle);
		
		if(texCoordHandle >= 0)
			GLES20.glDisableVertexAttribArray(texCoordHandle);		
	}
	
	public void drawArrays(int positionHandle, int texCoordHandle) {
		drawArrays(positionHandle, texCoordHandle, GLES20.GL_TRIANGLES);
	}
	
	public void drawArrays(int positionHandle, int texCoordHandle, int mode) {
		if(positionHandle >= 0) {
			GLES20.glEnableVertexAttribArray(positionHandle);
			GLES20.glVertexAttribPointer(positionHandle, 2, GLES20.GL_FLOAT, false, 0, getPositions());
		}
		
		if(texCoordHandle >= 0) {
			GLES20.glEnableVertexAttribArray(texCoordHandle);
			GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 0, getTexCoords());
		}
		
		GLES20.glDrawArrays(mode, 0, getPositions().capacity() / 2);
		
		if(positionHandle >= 0)
			GLES20.glDisableVertexAttribArray(positionHandle);
		
		if(texCoordHandle >= 0)
			GLES20.glDisableVertexAttribArray(texCoordHandle);					
	}
	
}
