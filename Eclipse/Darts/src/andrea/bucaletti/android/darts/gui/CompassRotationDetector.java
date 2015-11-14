package andrea.bucaletti.android.darts.gui;

import andrea.bucaletti.android.lib.motion.RotationDetector;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.widget.TextView;

public class CompassRotationDetector extends RotationDetector implements SensorEventListener {

	private float[] rotationMatrix, inclinationMatrix;
	private float[] gravity, magfield;
	private float[] deviceOrientation;
	private float rotateX, rotateZ;
	
	private SensorManager manager;
	private Sensor accelerometer, compass;
	
	public CompassRotationDetector(Context context) {
		SensorManager manager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
		
		accelerometer = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		compass = manager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
		
		rotationMatrix = new float[9];
		inclinationMatrix = new float[9];
		
		gravity = new float[3];
		magfield = new float[3];
		
		deviceOrientation = new float[3];
		
		manager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);
		manager.registerListener(this, compass, SensorManager.SENSOR_DELAY_GAME);
	}
	
	@Override
	public float getRotateX() {
		//return (float)(-Math.toDegrees(deviceOrientation[2]) + 90);
		return rotateX;
	}

	@Override
	public float getRotateY() {
		return (float)Math.toDegrees(deviceOrientation[0]);
	}

	@Override
	public float getRotateZ() {
		//return (float)-Math.toDegrees(deviceOrientation[1]);
		return rotateZ;
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		if(event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			float alpha = 0.9f;
			
			gravity[0] = gravity[0] * alpha - (1 - alpha) * event.values[0];
			gravity[1] = gravity[1] * alpha - (1 - alpha) * event.values[1];
			gravity[2] = gravity[2] * alpha - (1 - alpha) * event.values[2];
			
			float f = (float)Math.sqrt(gravity[0] * gravity[0] + gravity[1] * gravity[1] + gravity[2] * gravity[2]);
			
			rotateZ = (float)Math.toDegrees(Math.atan(gravity[0] / gravity[1])) - 90;
			rotateX = (float)-(Math.toDegrees(Math.acos(gravity[2] / f)) - 90);
			
			if(gravity[1] > 0)
				rotateZ += 180;
		} else {
			float alpha = 0.5f;
			magfield[0] = magfield[0] * alpha - (1 - alpha) * event.values[0];
			magfield[1] = magfield[1] * alpha - (1 - alpha) * event.values[1];
			magfield[2] = magfield[2] * alpha - (1 - alpha) * event.values[2];
		}
		
		SensorManager.getRotationMatrix(rotationMatrix, inclinationMatrix, gravity, magfield);
		SensorManager.getOrientation(rotationMatrix, deviceOrientation);		
	}

}
