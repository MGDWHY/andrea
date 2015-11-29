package andrea.bucaletti.android.fireworks;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class BasicGLSurfaceView extends GLSurfaceView {
	
	FireworksRenderer mRenderer;
	
	SoundPool mSoundPool;
	
	int popSoundID;
	
	long timeDownStart;
	
	public BasicGLSurfaceView(Context context, FireworksRenderer renderer) {
		super(context);
		
		this.mRenderer = renderer;
		
		setEGLContextClientVersion(2);
		setRenderer(mRenderer);
		
		mSoundPool = new SoundPool(10, AudioManager.STREAM_MUSIC, 0);
		popSoundID = mSoundPool.load(context, R.raw.firework, 1);
		
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			timeDownStart = System.currentTimeMillis();
			break;
		case MotionEvent.ACTION_UP:
			float timePressed = (System.currentTimeMillis() - timeDownStart) / 1000.0f;
			
			if(this.isSoundEffectsEnabled())
				mSoundPool.play(popSoundID, 1.0f, 1.0f, 1, 0, 1.0f);
			
			mRenderer.addFirework(event.getX(), event.getY(), timePressed);
			
			break;
		}
		return true;
	}

}
