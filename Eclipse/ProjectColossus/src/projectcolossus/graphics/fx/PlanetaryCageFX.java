package projectcolossus.graphics.fx;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Collections;

import andrea.bucaletti.android.lib.Timer;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import andrea.bucaletti.android.lib.vecmath.Vec4f;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;
import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.PRNG;
import projectcolossus.util.Util;

public class PlanetaryCageFX extends PlanetFX {
	
	/*
	 * Cage vertex: x, y, startAngle, speed
	 * 4 floats 16 bytes
	 */
	private static final int CAGE_VERTEX_SIB = 16;
	
	private static final int CAGE_CIRCLES = 3;
	private static final int CAGE_POINTS_PER_CIRCLE = 180;
	
	private static final int CAGE_POINT_SIZE = 24;
	
	protected GLMatrixStack mStack;
	
	protected float pointSize;
	
	protected int texCage;
	protected int prgPlanetCage;
	protected ByteBuffer vboCage;
	
	protected Timer timer;

	public PlanetaryCageFX() {
		super();
		this.mStack = new GLMatrixStack(1);
		this.timer = new Timer();
		this.pointSize = Util.pixelToDP(CAGE_POINT_SIZE, ResourceLoader.getDisplayDensity());	
	}
	
	@Override
	protected void update() {
		vboCage = generateCage();
	}
	
	@Override
	public void draw(float[] mvpMatrix, Planet planet) {
		
		int hTexture, hMVP, hColor, hPosition, hTime, hPointSize;
		
		if(vboCage == null)
			update();
		
		if(prgPlanetCage == 0)
			prgPlanetCage = ResourceLoader.getProgram(Constants.PRG_FX_PLANET_CAGE);
		
		if(texCage == 0)
			texCage = ResourceLoader.getTexture(Constants.TEX_PLANET_CAGE);
		
		float[] color = (float[])getAttribute(Constants.FXATTRIB_COLOR0);
		
		mStack.set(mvpMatrix);

		hMVP = GLES20.glGetUniformLocation(prgPlanetCage, "in_MVP");
		hTime = GLES20.glGetUniformLocation(prgPlanetCage, "in_Time");
		hTexture = GLES20.glGetUniformLocation(prgPlanetCage, "in_Texture");
		hColor = GLES20.glGetUniformLocation(prgPlanetCage, "in_Color");
		hPointSize = GLES20.glGetUniformLocation(prgPlanetCage, "in_Size");
		
		hPosition = GLES20.glGetAttribLocation(prgPlanetCage, "in_Position");
				
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glUseProgram(prgPlanetCage);
		
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texCage);
		
			GLES20.glUniformMatrix4fv(hMVP, 1, false, mStack.current(), 0);
			GLES20.glUniform1f(hTime, timer.time());
			GLES20.glUniform1i(hTexture, 0);
			GLES20.glUniform1f(hPointSize, pointSize);
			GLES20.glUniform3fv(hColor, 1, color, 0);
		
			
			GLES20.glEnableVertexAttribArray(hPosition);
			GLES20.glVertexAttribPointer(hPosition, 4, GLES20.GL_FLOAT, false, 0, vboCage);

			GLES20.glDrawArrays(GLES20.GL_POINTS, 0, CAGE_CIRCLES * CAGE_POINTS_PER_CIRCLE);
			
			GLES20.glDisableVertexAttribArray(hPosition);
				
	
		GLES20.glUseProgram(0);
		GLES20.glDisable(GLES20.GL_BLEND);
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		
	}
	
	private ByteBuffer generateCage() {
		ByteBuffer data = ByteBuffer.allocateDirect(CAGE_VERTEX_SIB * CAGE_CIRCLES * CAGE_POINTS_PER_CIRCLE).order(ByteOrder.nativeOrder());
		FloatBuffer ff = data.asFloatBuffer();
		ArrayList<Vec4f> points = new ArrayList<Vec4f>();
		
		float angleOffset = timer.time() * 1.0f;

		for(int i = 0; i < CAGE_CIRCLES - 1; i++) {	
			float beta = (float)(Math.PI / (CAGE_CIRCLES - 1) * i + angleOffset);		
			for(int j = 0; j < CAGE_POINTS_PER_CIRCLE; j++) {
				float alpha = (float)(2 * Math.PI / CAGE_POINTS_PER_CIRCLE * j);
				
				float x = (float) (Math.cos(alpha) * Math.cos(beta));
				float y = (float) Math.sin(alpha);
				float z = (float) (Math.cos(alpha) * Math.sin(beta));
				float w = (z + 1) / 2.0f;
				
				points.add(new Vec4f(x, y, z, w));	
				
				
			}
		}
		
		for(int j = 0; j < CAGE_POINTS_PER_CIRCLE; j++) {
			float alpha = (float)(2 * Math.PI / CAGE_POINTS_PER_CIRCLE * j + angleOffset);
			
			float x = (float) Math.cos(alpha);
			float y = 0;
			float z = (float) Math.sin(alpha);
			float w = (z + 1) / 2.0f;
			
			points.add(new Vec4f(x, y, z, w));	
			
		}
		
		Collections.sort(points, Vec4f.ZComparator);
		
		for(Vec4f p : points)
			ff.put(p.getComponents());
		
		return data;
		
	}	
}
