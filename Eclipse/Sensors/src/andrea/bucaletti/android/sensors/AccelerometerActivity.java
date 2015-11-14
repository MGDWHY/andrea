package andrea.bucaletti.android.sensors;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;
import android.app.Activity;
import android.content.Context;

public class AccelerometerActivity extends Activity implements SensorEventListener {
	
	private SensorManager sensorManager;
	private Sensor sensor;
	
	private TextView x, y, z;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_accelerometer);
        
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        x = (TextView) findViewById(R.id.accX);
        y = (TextView) findViewById(R.id.accY);
        z = (TextView) findViewById(R.id.accZ);        
    }
    
    @Override
    public void onResume() {
    	super.onResume();
        sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL);    	
    }
    
    @Override
    public void onPause() {
    	super.onPause();
    	sensorManager.unregisterListener(this);
    }
    
    public void onSensorChanged(SensorEvent event) {
    	x.setText(event.values[0] + " m/s");
    	y.setText(event.values[1] + " m/s");
    	z.setText(event.values[2] + " m/s");
    }

	public void onAccuracyChanged(Sensor sensor, int accuracy) {

	}
}
