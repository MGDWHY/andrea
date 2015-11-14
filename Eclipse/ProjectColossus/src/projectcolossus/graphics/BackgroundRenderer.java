package projectcolossus.graphics;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Random;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.Util;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import andrea.bucaletti.projectcolossus.R;
import android.content.res.Resources;
import android.graphics.RectF;
import android.opengl.GLES20;
import android.util.Log;

public class BackgroundRenderer {
	
	/* GENERATE BACKGROUND WITH NEBULAS AND STARS */
	
	/*
	 * Star vector: x y z r g b size
	 * Size: 7 * 4 = 28 bytes
	 */
	private static final int STAR_VECTOR_SIB = 28;
	
	/*
	 * Nebula point vector: x y z r g b size
	 * Size: 7 * 4 = 28 bytes
	 */
	private static final int NEBULA_POINT_VECTOR_SIB= 28;
	
	private static final float STAR_DENSITY = 0.005f;
	
	private static final float MIN_Z = -200;
	private static final float MAX_Z = 0;
	
	private static final float MIN_SIZE = 2;
	private static final float MAX_SIZE = 6;
	
	/* Nebula parameters */
	private static final int MIN_NEBULAS = 20;
	private static final int MAX_NEBULAS = 21;
	
	private static final float NEBULA_POINT_MIN_SIZE = 10;
	private static final float NEBULA_POINT_MAX_SIZE = 40;
	
	private static final int NEBULA_MIN_POINTS = 100;
	private static final int NEBULA_MAX_POINTS = 500;
	
	private static final float[][] NEBULA_COLORS =
		{
			{1, 0.2f, 0.2f}, // red
			{1, 0.2f, 1} // violet
		};
	
	private static final float NEBULA_SPREADING = 30;
	
	protected Vec2f mapSize;
	
	protected RectF bounds;
	
	protected int starsProgram, nebulaProgram;
	
	protected GLVertexBufferObject vboStars, vboNebulas;
	protected int starsVCount, nebulasVCount;
	
	protected int texStar, texNebula, texNoise;
	
	protected Random rnd;

	public BackgroundRenderer(Resources resources, Vec2f mapSize) {
		
		this.mapSize = mapSize;
		
		rnd = new Random();
		
		bounds = new RectF(
				-GameViewInputManager.MAP_EDGE_GAP, 
				-GameViewInputManager.MAP_EDGE_GAP, 
				mapSize.getX() + GameViewInputManager.MAP_EDGE_GAP,
				mapSize.getY() + GameViewInputManager.MAP_EDGE_GAP);
		
		/* Generating stars */
		
		vboStars = new GLVertexBufferObject(generateStars(), GLES20.GL_STATIC_DRAW);	
		
		/* Generating nebulas */
		
		int numNebulas = randomInt(MIN_NEBULAS, MAX_NEBULAS);
		ByteBuffer[] nebulasVData = new ByteBuffer[numNebulas];
		int[] nebulaVCount = new int[numNebulas];
		
		for(int i = 0; i < numNebulas; i++) {
			nebulasVData[i] = generateNebula();
			nebulaVCount[i] = nebulasVData[i].capacity() / NEBULA_POINT_VECTOR_SIB; 
			nebulasVCount += nebulaVCount[i];
		}
		
		vboNebulas = new GLVertexBufferObject();
		vboNebulas.setBufferData(nebulasVCount * NEBULA_POINT_VECTOR_SIB, GLES20.GL_STATIC_DRAW);
		
		int offset = 0;	
		for(int i = 0; i < numNebulas; i++) {
			vboNebulas.setBufferSubData(nebulasVData[i], 1, offset);
			offset += nebulaVCount[i] * NEBULA_POINT_VECTOR_SIB;
		}
		
		/* programs and textures */
		starsProgram = GLU.loadProgram(R.string.star_vs, R.string.star_fs, resources);
		nebulaProgram = GLU.loadProgram(R.string.nebula_vs, R.string.nebula_fs, resources);
		
		texStar = GLU.loadTexture2D(R.drawable.star, resources);
		texNebula = ResourceLoader.getTexture(Constants.TEX_NEBULA);
		texNoise = GLU.loadTexture2D(R.drawable.noise, resources);
	}
	
	public void drawStars(float[] vpMatrix) {
	
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
		
		GLES20.glUseProgram(starsProgram);

		/* Draw nebulas */
		drawVBO(vboNebulas, nebulasVCount, texNebula, vpMatrix);
		
		/* Draw stars */
		
		drawVBO(vboStars, starsVCount, texStar, vpMatrix);
	
		GLES20.glUseProgram(0);

		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		GLES20.glDisable(GLES20.GL_BLEND);
	}
	
	private void drawVBO(GLVertexBufferObject vbo, int vCount, int texture, float[] vpMatrix) {
		
		int mvpHandle, textureHandle, positionHandle, colorHandle, sizeHandle;
		
		vbo.bind();
		
		mvpHandle = GLES20.glGetUniformLocation(starsProgram, "in_MVP");
		textureHandle = GLES20.glGetUniformLocation(starsProgram, "in_Texture");
		
		positionHandle = GLES20.glGetAttribLocation(starsProgram, "in_Position");
		colorHandle = GLES20.glGetAttribLocation(starsProgram, "in_Color");
		sizeHandle = GLES20.glGetAttribLocation(starsProgram, "in_Size");
		
		/* Draw stars */
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture);
		
		GLES20.glUniformMatrix4fv(mvpHandle, 1, false, vpMatrix, 0);
		GLES20.glUniform1i(textureHandle, 0);
		
		GLES20.glEnableVertexAttribArray(positionHandle);
		GLES20.glEnableVertexAttribArray(colorHandle);
		GLES20.glEnableVertexAttribArray(sizeHandle);
		
		GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 28, 0);
		GLES20.glVertexAttribPointer(colorHandle, 3, GLES20.GL_FLOAT, false, 28, 12);
		GLES20.glVertexAttribPointer(sizeHandle, 1, GLES20.GL_FLOAT, false, 28, 24);
		
		GLES20.glDrawArrays(GLES20.GL_POINTS, 0, vCount);
		
		GLES20.glDisableVertexAttribArray(positionHandle);
		GLES20.glDisableVertexAttribArray(colorHandle);
		GLES20.glDisableVertexAttribArray(sizeHandle);
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		vbo.unbind();		
	}
	
	private ByteBuffer generateStars() {
		starsVCount = (int)(mapSize.getX() * mapSize.getY() * STAR_DENSITY);
		ByteBuffer data = ByteBuffer.allocateDirect(starsVCount * STAR_VECTOR_SIB).order(ByteOrder.nativeOrder());
		FloatBuffer ff = data.asFloatBuffer();
		
		for(int i = 0; i < starsVCount; i++) {
			ff.put(randomPoint().toArray());
			ff.put(randomStarColor());
			ff.put(randomFloat(Util.pixelToDP(randomFloat(MIN_SIZE, MAX_SIZE), ResourceLoader.getDisplayDensity())));
		}
		
		return data;	
	}
	
	private ByteBuffer generateNebula() {
		
		int totalPoints = rnd.nextInt(NEBULA_MAX_POINTS - NEBULA_MIN_POINTS) + NEBULA_MIN_POINTS;
		ByteBuffer data = ByteBuffer.allocateDirect(totalPoints * NEBULA_POINT_VECTOR_SIB).order(ByteOrder.nativeOrder());
		FloatBuffer ff = data.asFloatBuffer();
		ArrayList<Vec3f> points = generateNubulaPoints(randomPoint(), totalPoints);
		
		for(Vec3f point : points) {
			ff.put(point.toArray()); // position
			ff.put(randomNebulaColor()); // color
			ff.put(Util.pixelToDP(randomFloat(NEBULA_POINT_MIN_SIZE, NEBULA_POINT_MAX_SIZE), ResourceLoader.getDisplayDensity()));
		}	
		
		return data;
	}
	
	private ArrayList<Vec3f> generateNubulaPoints(Vec3f startPoint, int totalPoints) {
		
		ArrayList<Vec3f> points = new ArrayList<Vec3f>();
		
		points.add(startPoint);
		
		if(points.size() == 0)
			throw new IllegalArgumentException("generateNebulaPoints(): At least one point must be set");
		
		while(totalPoints > points.size()) {
			Vec3f startingPoint = points.get(rnd.nextInt(points.size()));
			Vec3f spreadVector = new Vec3f(
					randomSignum() * randomFloat(NEBULA_SPREADING),
					randomSignum() * randomFloat(NEBULA_SPREADING),
					randomSignum() * randomFloat(NEBULA_SPREADING)
			);
			
			points.add(startingPoint.add(spreadVector));					
		}
		
		return points;
	}
	
	private int randomInt(int min, int max) {
		return rnd.nextInt(max - min) + min;
	}
	
	private float randomFloat(float min, float max) {
		return rnd.nextFloat() * (max - min) + min;
	}
	
	private float randomFloat(float max) {
		return rnd.nextFloat() * max;
	}
	
	public int randomSignum() {
		if(rnd.nextFloat() >= 0.5f) return 1;
		else return -1;
	}
	
	private Vec3f randomPoint() {
		return new Vec3f(
				randomFloat(bounds.left, bounds.right),
				randomFloat(bounds.top, bounds.bottom),
				randomFloat(MIN_Z, MAX_Z)	
		);
	}
	
	private float[] randomStarColor() {
		float c[] = new float[3];
		c[0]= 0.5f + rnd.nextFloat() * 0.5f;
		c[1] = 0.5f + rnd.nextFloat() * 0.5f;
		c[2] = 1;	
		return c;
	}
	
	private float[] randomNebulaColor() {
		return NEBULA_COLORS[rnd.nextInt(NEBULA_COLORS.length)];
	}

}
