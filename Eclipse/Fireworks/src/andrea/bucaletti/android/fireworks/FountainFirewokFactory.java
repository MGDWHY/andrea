package andrea.bucaletti.android.fireworks;

import java.nio.ByteBuffer;

import android.util.FloatMath;

public class FountainFirewokFactory extends FireworkFactory {
	
	private static final int SPARKS_PER_FLOW = 20;
	
	private static final int NUM_FLOWS = 20;
	
	private final float MIN_FIREWORK_SPEED = 50;
	private final float MAX_FIREWORK_SPEED = 200;
	private final float JITTER_FIREWORK_SPEED = MAX_FIREWORK_SPEED - MIN_FIREWORK_SPEED;
	
	private final float TIME_TO_CHARGE = 2.0f;	

	@Override
	public ByteBuffer generate(Params params) {
		
		int numSparks = SPARKS_PER_FLOW * NUM_FLOWS;
		
		ByteBuffer result = SparkManager.createSparkBuffer(numSparks);
		
		float[] colorVec = new float[3];
		
		float startAngle = (float)(random() * Math.PI * 2);
		float incAngle = (float)(Math.PI * 2 / NUM_FLOWS);
		
		float velocity = MIN_FIREWORK_SPEED + Math.min(JITTER_FIREWORK_SPEED, params.timePressed / TIME_TO_CHARGE * JITTER_FIREWORK_SPEED );
		
		for(int i = 0; i < NUM_FLOWS; i++) {
			
			float angle = startAngle + incAngle * i;
			
			float crazyness = random(0.5f, 1.0f);
			
			float vx = FloatMath.cos(angle) * velocity * crazyness;
			float vy = FloatMath.sin(angle) * velocity * crazyness;
			
			SparkManager.SparkDescriptor desc = new SparkManager.SparkDescriptor();
			
			for(int j = 0; j < SPARKS_PER_FLOW; j++) {
				float scaling0 = ((float)(SPARKS_PER_FLOW - j + 1) / SPARKS_PER_FLOW) * 0.5f + 0.5f;
				
				FireworkFactory.toFloatColor(brighterColor(params.color0, scaling0), colorVec);
				
				desc.x = params.x;
				desc.y = params.y;
				desc.vx = vx * scaling0;
				desc.vy = vy * scaling0;
				desc.r = colorVec[0];
				desc.g = colorVec[1];
				desc.b = colorVec[2];
				desc.size = 3 * scaling0;
				
				SparkManager.writeSpark(result, desc);
				
			}
		}	
		
		
		return result;
	}
}
