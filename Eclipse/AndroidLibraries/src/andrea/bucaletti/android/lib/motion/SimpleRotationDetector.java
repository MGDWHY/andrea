package andrea.bucaletti.android.lib.motion;

import andrea.bucaletti.android.lib.Timer;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public class SimpleRotationDetector extends RotationDetector {
	
	private static final float DEFAULT_NOISE_SCALING = 1.5f;
	
	private SensorManager sensorManager;
	private Sensor accelerometer;
	
	private float velRotateY;
	private float rotateX, rotateY, rotateZ;

	private float alpha = 0.8f;	

	private float noiseScaling = DEFAULT_NOISE_SCALING;
	
	private AccelerometerListener listener;

	
	private Vec3f values = new Vec3f();
	private Vec3f gravity = new Vec3f();
	private Vec3f noise = new Vec3f();
	private Vec3f scaledAbsoluteNoise = new Vec3f();
	private Vec3f linearAcceleration = new Vec3f();
	
	private int sample = 0;
	private int maxSamples = 64;
	
	private boolean calibrating = false;
	
	private Timer timer;
	
	public SimpleRotationDetector(Context context) {
		
		sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		
		accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		
		listener = new AccelerometerListener();
		
		sensorManager.registerListener(listener, accelerometer, SensorManager.SENSOR_DELAY_GAME);
		
		this.timer = new Timer();
		
	}
	
	/*
	 * Rotazioni degli assi X e Z. 
	 * I valori sono stati "aggiustati" per essere conformi al sistema di riferimento cartesiano di OpenGL
	 */
	public float getRotateZ() { return rotateZ; }
	public float getRotateX() { return rotateX; }
	public float getRotateY() { return rotateY; }
	
	public void calibrate(int maxSamples) {
		this.maxSamples = maxSamples;
		calibrating = true; 
		sample = 0;
	}
	
	public void setAverageNoise(Vec3f noise) {
		this.noise = noise;
	}

	private class AccelerometerListener implements SensorEventListener {	
		
		@Override
		public void onSensorChanged(SensorEvent event) {
			
			float dt = timer.dt();
			values.set(event.values[0], event.values[1], event.values[2]);
			
			if(calibrating) {
				
				// Togli la forza di gravità dai campioni
				gravity = gravity.scale(alpha).sub(values.scale(1 - alpha));
				values = values.add(gravity);				
				
				// Calcola rumore medio dell'accelerometro su #maxSamples campioni
				if(sample < maxSamples) {
					noise = noise.add(values);
					sample++;
				} else if(sample == maxSamples) {
					noise = noise.scale(1.0f / maxSamples);
					scaledAbsoluteNoise = new Vec3f((float)Math.abs(noise.x), (float)Math.abs(noise.y),(float)Math.abs(noise.z)).scale(noiseScaling);
					calibrating = false;
				}
			} else {
				
				
				// values = values.sub(noise);
				
				gravity =  gravity.scale(alpha).sub(values.scale(1 - alpha));				
				linearAcceleration = values.add(gravity);
				
				
				// rotateZ
				rotateZ = (float)Math.toDegrees(Math.atan(gravity.x / gravity.y)) - 90;
				if(gravity.y > 0)
					rotateZ += 180;
				
				// rotateX
				float f = (float)Math.sqrt(gravity.x * gravity.x + gravity.y * gravity.y + gravity.z * gravity.z);
				rotateX = (float)-(Math.toDegrees(Math.acos(gravity.z / f)) - 90);
				

				// rotateY
				float accY;
				/*if(Math.abs(linearAcceleration.y) > scaledAbsoluteNoise.y ) {
					if(linearAcceleration.y > 0)
						accY = linearAcceleration.y - scaledAbsoluteNoise.y ;
					else
						accY = linearAcceleration.y + scaledAbsoluteNoise.y;
				} else
					accY = 0;

				velRotateY -= accY * dt;
				rotateY += (float)Math.toDegrees(velRotateY * dt);*/			
				
				if(Math.abs(gravity.y) > 2.0f) {
					if(gravity.x > 0)
						rotateY += (gravity.y - 2.0f);
					else
						rotateY -= (gravity.y + 2.0f);
				}
				
				Log.d("GLInfo", gravity.y + " ");
				

			}
		}
		
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
	
		}		
		
	}
}
