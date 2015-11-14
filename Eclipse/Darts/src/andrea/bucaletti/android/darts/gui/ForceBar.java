package andrea.bucaletti.android.darts.gui;

import java.nio.FloatBuffer;

import andrea.bucaletti.android.lib.g2d.Rectangle;

public class ForceBar extends Rectangle {
	
	private float ratio;
	
	public ForceBar(float x, float y, float width, float height) {
		super(x, y, width, height);
	}
	
	public void setForce(float force) {
		this.ratio = force / GameUI.MAX_FORCE;
	}
	
	public FloatBuffer getPositions() {

		// write bottom right vertex X
		positions.position(2); 
		positions.put(x + width * ratio);
		
		// write top right vertex x
		positions.position(4);
		positions.put(x + width * ratio);
		
		positions.position(0);

		return positions;
	}
	public FloatBuffer getTexCoords() {

		// write bottom right vertex tu
		texCoords.position(2);
		texCoords.put(ratio);
		
		// write top right vertex tu
		texCoords.position(4);
		texCoords.put(ratio);
		
		texCoords.position(0);

		return texCoords;
	}
	
	
}
