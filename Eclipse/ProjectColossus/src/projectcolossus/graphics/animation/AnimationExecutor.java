package projectcolossus.graphics.animation;

import java.util.ArrayList;

import projectcolossus.util.LockableArrayList;

import andrea.bucaletti.android.lib.Timer;
import android.annotation.SuppressLint;

public class AnimationExecutor {
	
	private Timer timer;
	
	private LockableArrayList<Animation> lstAnimQueue;
	private LockableArrayList<Animation> lstAnimRunning;
	
	public AnimationExecutor() {
		timer = new Timer();
		lstAnimQueue = new LockableArrayList<Animation>();
		lstAnimRunning = new LockableArrayList<Animation>();
	}
	/**
	 * Serial animation execution
	 * @param anim
	 */
	public synchronized void queueAnimation(Animation anim) { lstAnimQueue.add(anim); }
	
	/**
	 * Parallel animation execution
	 * @param anim
	 */
	public synchronized void runAnimation(Animation anim) { lstAnimRunning.add(anim); }
	
	public synchronized void update() {
		
		if(lstAnimRunning.size() == 0 && lstAnimQueue.size() > 0) {
			lstAnimRunning.add(lstAnimQueue.get(0));
			lstAnimQueue.remove(0);
		}
		
		float dt = timer.dt();
		
		for(Animation anim : lstAnimRunning) {
			
			if(!anim.started())
				anim.start();
			
			anim.onUpdate(dt);
		}
		
	}
	
	@SuppressLint("WrongCall")
	public synchronized void draw(float[] vpMatrix) {
		
		lstAnimRunning.lock();
		
		for(Animation anim : lstAnimRunning) {
			anim.onDraw(vpMatrix);
			
			if(anim.stopped())
				lstAnimRunning.remove(anim);
			
		}
		lstAnimRunning.unlock();
	}
	
}
