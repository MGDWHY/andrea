package com.erbuka.fireworks;

import java.nio.ByteBuffer;

import android.util.FloatMath;

public class NormalFireworkFactory extends FireworkFactory {

	private final int MIN_FIREWORK_SPARKS = 50;
	private final int MAX_FIREWORK_SPARKS = 100;
	
	private final float MIN_FIREWORK_SPEED = 50;
	private final float MAX_FIREWORK_SPEED = 200;
	private final float JITTER_FIREWORK_SPEED = MAX_FIREWORK_SPEED - MIN_FIREWORK_SPEED;

	private final float TIME_TO_CHARGE = 2.0f;	
	
	@Override
	public ByteBuffer generate(Params params) {
		
		float[] colorVec = new float[3];
		float nsFactor = (1 + params.timePressed * 3.0f);
		int numSparks = Math.round(nsFactor * random() * (MAX_FIREWORK_SPARKS - MIN_FIREWORK_SPARKS) + MIN_FIREWORK_SPARKS);
		
		ByteBuffer result = SparkManager.createSparkBuffer(numSparks);
		
		float startAngle = (float)(random() * Math.PI * 2);
		float incAngle = (float)(Math.PI * 2 / numSparks);
		
		synchronized (this) {	
			float velocity = MIN_FIREWORK_SPEED + Math.min(JITTER_FIREWORK_SPEED, params.timePressed / TIME_TO_CHARGE * JITTER_FIREWORK_SPEED );
			SparkManager.SparkDescriptor desc = new SparkManager.SparkDescriptor();
			for(int i = 0; i < numSparks; i++) {			
				
				float angle = startAngle + incAngle * i;
				
				float crazyness = random(0.1f, 1.0f);
				float brightness = random();
				
				FireworkFactory.toFloatColor(brighterColor(params.color0, brightness), colorVec);				
				
				desc.x = params.x;
				desc.y = params.y;
				
				desc.vx = (float)Math.cos(angle) * velocity * crazyness;
				desc.vy = (float)Math.sin(angle) * velocity * crazyness;
				
				desc.r = colorVec[0];
				desc.g = colorVec[1];
				desc.b = colorVec[2];

				desc.size = random(1.0f, 6.0f);				
				
				SparkManager.writeSpark(result, desc);
			}
		}		
		
		
		return result;
	}
}
