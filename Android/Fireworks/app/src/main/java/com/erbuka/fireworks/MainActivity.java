package com.erbuka.fireworks;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ListView;
import android.app.Activity;

public class MainActivity extends Activity {
	
	private ListView mBackgroundChangeView;
	
	private BasicGLSurfaceView mGLSView;
	private FireworksRenderer mRenderer;
	
	private MenuItem mMenuBackGround;
	private MenuItem mMenuSound;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        mRenderer = new FireworksRenderer(this.getApplication());
        mGLSView = new BasicGLSurfaceView(this.getApplication(), mRenderer);
        mBackgroundChangeView = new ListView(this.getApplication());
        setContentView(mGLSView);
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        mGLSView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLSView.onResume();
    }
    
    @Override
	public boolean onCreateOptionsMenu(Menu menu) {
    	
    	mMenuBackGround = menu.add(Menu.NONE, Menu.NONE, 0, "Change Background");
    	mMenuSound = menu.add(Menu.NONE, Menu.NONE, Menu.FLAG_PERFORM_NO_CLOSE, "Sound");
    	
    	mMenuSound.setCheckable(true);
    	mMenuSound.setChecked(true);
    	
    	return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	if(item == mMenuSound) {
    		mMenuSound.setChecked(!mMenuSound.isChecked());
    		mGLSView.setSoundEffectsEnabled(mMenuSound.isChecked());
    	} else if(item == mMenuBackGround) {
    		setContentView(mBackgroundChangeView);
    	}
    	return false;
    }
}
