package projectcolossus.graphics.fx;

import projectcolossus.gamelogic.Planet;
import android.opengl.Matrix;
import android.util.SparseArray;

public abstract class PlanetFX {
	
	private int frameID;
	private SparseArray<Object> attributes;
	
	public PlanetFX() {
		this.attributes = new SparseArray<Object>();
	}
	
	public void setAttribute(int attributeID, Object data) {
		attributes.put(attributeID, data);
	}
	
	public void unsetAttribute(int attributeID) {
		attributes.remove(attributeID);
	}
	
	public Object getAttribute(int attributeID) {
		return attributes.get(attributeID);
	}
	
	/*
	 * This function will update the effect only if
	 * the the frameID is different from the 
	 * one previously stored.
	 * This is to prevent multiple updates during the
	 * same draw frame (for instance if the effect is used
	 * 2 times, it would be updateted 2 times)
	 */
	public void update(int frameID) {
		if(this.frameID != frameID) {
			this.frameID = frameID;
			update();
		}
	}
	
	protected abstract void update();
	
	public abstract void draw(float[] mvpMatrix, Planet planet);
	
	public void draw(float[] vpMatrix, float[] mMatrix, Planet planet) {
		if(mMatrix == null)
			draw(vpMatrix, planet);
		else {
			float[] mvpMatrix = new float[16];
			Matrix.multiplyMM(mvpMatrix, 0, vpMatrix, 0, mMatrix, 0);
			draw(mvpMatrix, planet);
		}
	}
	
}
