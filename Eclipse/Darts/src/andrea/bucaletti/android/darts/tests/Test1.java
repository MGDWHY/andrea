package andrea.bucaletti.android.darts.tests;


import andrea.bucaletti.darts.R;
import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.TextView;

public class Test1 extends Activity implements SensorEventListener {
	
	private float[] rotationMatrix, inclinationMatrix;
	private float[] gravity, magfield;
	private float[] deviceOrientation;
	
	private SensorManager manager;
	private Sensor accelerometer, compass;
	
	private TextView txtAngleX, txtAngleY, txtAngleZ;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.test1);
		
		txtAngleX = (TextView) findViewById(R.id.txtAngleX);
		txtAngleY = (TextView) findViewById(R.id.txtAngleY);
		txtAngleZ = (TextView) findViewById(R.id.txtAngleZ);
		
		SensorManager manager = (SensorManager)getApplicationContext().getSystemService(Context.SENSOR_SERVICE);
		
		accelerometer = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		compass = manager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
		
		rotationMatrix = new float[9];
		inclinationMatrix = new float[9];
		
		gravity = new float[3];
		magfield = new float[3];
		
		deviceOrientation = new float[3];
		
		manager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);
		manager.registerListener(this, compass, SensorManager.SENSOR_DELAY_GAME);
		
		setTitle("Angle Test");
		
	}
	
	@Override
	protected void onStart() {
		
		super.onStart();
		
		final Runnable updateAction = new Runnable () {
			public void run() {
				txtAngleZ.setText("Z: "+ (int)Math.toDegrees(deviceOrientation[2]));
				txtAngleX.setText("X: "+ (int)Math.toDegrees(deviceOrientation[0]));
				txtAngleY.setText("Y: "+ (int)Math.toDegrees(deviceOrientation[1]));
			}
		};

		new Thread() {
			public void run() {
				while(true) {
					SensorManager.getRotationMatrix(rotationMatrix, inclinationMatrix, gravity, magfield);
					SensorManager.getOrientation(rotationMatrix, deviceOrientation);
					
					runOnUiThread(updateAction);				
					try {Thread.sleep(100);}
					catch(InterruptedException ex) { ex.printStackTrace();}
				}				
			}	
		}.start();		
			
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.test1, menu);
		return true;
	}
	
	public void resetTracker(View view) {
		
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		
		if(event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			gravity[0] = event.values[0];
			gravity[1] = event.values[1];
			gravity[2] = event.values[2];
		} else {
			magfield[0] = event.values[0];
			magfield[1] = event.values[1];
			magfield[2] = event.values[2];
		}
		
	}

}
