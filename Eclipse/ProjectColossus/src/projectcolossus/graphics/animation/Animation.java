package projectcolossus.graphics.animation;

public abstract class Animation {
	
	protected boolean started, stopped;
	
	protected EndCallback endCallback;

	public Animation() {
		this.started = this.stopped = false;
	}
	
	public void setEndCallback(EndCallback endCallback) {
		this.endCallback = endCallback;
	}
	
	public boolean started() {
		return started;
	}
	
	public boolean stopped() {
		return stopped;
	}
	
	public void start() {
		started = true;
		onStart();
	}
	
	public void stop() {
		stopped = true;
		onStop();
		if(endCallback != null)
			endCallback.onAnimationEnd(this);
	}
	
	public abstract void onStart();
	public abstract void onUpdate(float dt);
	public abstract void onDraw(float[] vpMatrix);
	public abstract void onStop();
	
	public static interface EndCallback {
		public void onAnimationEnd(Animation animation);
	}
	
}
