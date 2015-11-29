package com.erbuka.fireworks;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.graphics.Color;
import android.util.Log;



public class SparkManager {
	
	private static final float GRAVITY = -40;

	private static final float TIME_TO_LIVE = 5;	
	
	/*
	 * Sparks are allocated in buffers to be ready to use directly for Vertex Arrays
	 * Each element stores
	 * x float - 4
	 * y float - 4
	 * vx float - 4
	 * vy float - 4
	 * size float - 4
	 * r float - 4
	 * g float - 4
	 * b float - 4 
	 * alpha float - 4
	 * time float - 4
	 * Total length: 40 bytes
	 */
	
	public static int SPARK_SIZE_IN_BYTES = 40;
	
	public static int POSITION_OFFSET = 0;
	public static int SIZE_OFFSET = 16;
	public static int COLOR_OFFSET = 20;
	public static int ALPHA_OFFSET = 32;
	
	static ByteBuffer createSparkBuffer(int numSparks) {
	
		ByteBuffer result = ByteBuffer.allocateDirect(SPARK_SIZE_IN_BYTES * numSparks);
		result.order(ByteOrder.nativeOrder());
		result.position(0);
		return result;
	}
	
	static boolean updateSparkBuffer(ByteBuffer buffer, float dt) { // returns true if firework is dead
		buffer.position(0);
		
		SparkDescriptor desc = new SparkDescriptor();
		
		while(buffer.hasRemaining()) {
			SparkManager.peekSpark(buffer, desc);
			
			float lfr = desc.getLifeRatio();
			
			float lf2 = lfr * lfr * lfr;
			
			desc.time += dt;
			desc.x += dt * desc.vx;
			desc.y += dt * desc.vy + GRAVITY * dt;
			desc.vx *= (1.0 - lf2);
			desc.vy *= (1.0 - lf2);
			desc.alpha = 1.0f - lfr;
					
			
			if(!desc.isAlive())
				return false;
			
			SparkManager.writeSpark(buffer, desc);
		}
		
		return true;
	}
	
	static void writeSpark(ByteBuffer buffer, SparkDescriptor desc) {
		buffer.putFloat(desc.x);
		buffer.putFloat(desc.y);
		buffer.putFloat(desc.vx);
		buffer.putFloat(desc.vy);
		buffer.putFloat(desc.size);
		buffer.putFloat(desc.r);
		buffer.putFloat(desc.g);
		buffer.putFloat(desc.b);
		buffer.putFloat(desc.alpha);
		buffer.putFloat(desc.time);
	}
	
	static SparkDescriptor peekSpark(ByteBuffer buffer, SparkDescriptor result) {	
		return SparkManager.readSpark(buffer, result, true);
	}
	
	static SparkDescriptor readSpark(ByteBuffer buffer, SparkDescriptor result, boolean peek) {

		int prevPos = buffer.position();
		
		result.x = buffer.getFloat();
		result.y = buffer.getFloat();
		result.vx = buffer.getFloat();
		result.vy = buffer.getFloat();
		result.size = buffer.getFloat();
		result.r = buffer.getFloat();
		result.g = buffer.getFloat();
		result.b = buffer.getFloat();
		result.alpha = buffer.getFloat();
		result.time = buffer.getFloat();
		
		if(peek)
			buffer.position(prevPos);
		
		return result;
	}
	
	public static class SparkDescriptor {
		
		public float x, y;
		public float vx, vy;
		public float time;
		public float r, g, b;
		public float size;
		public float alpha;
		
		public float getTime() {
			return time;
		}
		
		public float getLifeRatio() {
			return time / TIME_TO_LIVE;
		}
		
		public boolean isAlive() {
			return time <= TIME_TO_LIVE;
		}		
	}
}
