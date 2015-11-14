package projectcolossus.graphics;

import projectcolossus.gamelogic.Vec2f;
import andrea.bucaletti.android.lib.vecmath.Vec3f;

public class Camera {
	
	protected Vec3f cameraPos;
	protected Vec2f mapSize;
	
	public Camera() {
		cameraPos = new Vec3f();
	}
	
	public Camera(float z) {
		cameraPos = new Vec3f(0, 0, z);
	}
	
	public void setPosition(Vec3f position) {this.cameraPos.set(position);}
	public Vec3f getPosition() { return cameraPos; }	
	
	public void setX(float x) { cameraPos.x = x; }
	public void setY(float y) { cameraPos.y = y; }
	public void setZ(float z) { cameraPos.z = z; }
	
	public float getX() { return cameraPos.x; }
	public float getY() { return cameraPos.y; }
	public float getZ() { return cameraPos.z; }
}
