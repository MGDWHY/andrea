package andrea.bucaletti.android.darts.tests;


import andrea.bucaletti.android.lib.motion.SimpleRotationDetector;
import andrea.bucaletti.darts.R;
import andrea.bucaletti.darts.R.layout;
import andrea.bucaletti.darts.R.menu;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.Log;
import android.view.Menu;
import android.view.View;

public class Test2 extends Activity {
	
	private RenderView renderView;
	private SimpleRotationDetector rotationDetector;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		rotationDetector = new SimpleRotationDetector(getApplicationContext());
		renderView = new RenderView(getApplicationContext(), rotationDetector);
		setTitle("Gravity Test");
		setContentView(renderView);
	}
	
	@Override
	protected void onStart() {
		
		super.onStart();
		
		final Runnable invalidateAction = new Runnable() {
			public void run() {
				renderView.invalidate();
			}
		};
		
		new Thread() {
			public void run() {
				while(true) {
					runOnUiThread(invalidateAction);
					try { Thread.sleep(20); }
					catch(InterruptedException ex) {}
				}
			}
			
		}.start();
	}


	
	private static class RenderView extends View {
		
		private Context context;
		private SimpleRotationDetector rotationDetector;
		
		private Paint paint, paint2;
		private Rect rect, rect2;
		
		public RenderView(Context context, SimpleRotationDetector detector) {
			super(context);
			this.context = context;
			this.rotationDetector = detector;
			
			paint = new Paint();
			paint.setColor(Color.RED);
			
			paint2 = new Paint();
			paint2.setColor(Color.YELLOW);
			
			rect = new Rect(-100, -100, 100, 100);
			rect2 = new Rect(-10, -100, 10, -80);
		}
		
		@Override
		protected void onDraw(Canvas canvas) {
			canvas.drawColor(Color.BLUE);
			canvas.translate(canvas.getWidth() / 2, canvas.getHeight() / 2);
			canvas.rotate(rotationDetector.getRotateZ());
			canvas.drawRect(rect, paint);
			canvas.drawRect(rect2, paint2);
		}
	}

}
