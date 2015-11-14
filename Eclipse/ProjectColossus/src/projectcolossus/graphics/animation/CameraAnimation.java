package projectcolossus.graphics.animation;

import projectcolossus.graphics.Camera;
import projectcolossus.graphics.GameViewInputManager;
import andrea.bucaletti.android.lib.vecmath.Vec3f;

public class CameraAnimation extends Animation {
	
	private float totalTime;
	private float currentTime;
	
	private Camera camera;
	
	private Vec3f startCameraPos, endCameraPos;

	protected GameViewInputManager inputManager;	
	
	public CameraAnimation(GameViewInputManager inputManager, float totalTime, Camera camera, Vec3f endCameraPos) {	
		super();	
		this.inputManager = inputManager;
		this.camera = camera;
		this.startCameraPos = camera.getPosition();
		this.endCameraPos = endCameraPos;
		this.totalTime = totalTime;
		this.currentTime = 0;
	}

	@Override
	public void onUpdate(float dt) {
		if(currentTime < totalTime) {
			currentTime += dt;
			
			if(currentTime > totalTime)
				currentTime = totalTime;
			
			float delta = currentTime / totalTime;
			
			camera.setPosition(startCameraPos.scale(1 - delta).add(endCameraPos.scale(delta)));			
		} else
			stop();
	}

	@Override
	public void onDraw(float[] vpMatrix) {}

	@Override
	public void onStart() {
		inputManager.setInputEnabled(false);
	}

	@Override
	public void onStop() {
		inputManager.updateFromCamera();
		inputManager.setInputEnabled(true);	
	}

}
