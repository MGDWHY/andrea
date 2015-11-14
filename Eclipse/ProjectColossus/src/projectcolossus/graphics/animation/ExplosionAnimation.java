package projectcolossus.graphics.animation;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.PRNG;
import projectcolossus.util.Util;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.android.lib.vecmath.Vec4f;
import android.media.SoundPool;
import android.opengl.GLES20;

public class ExplosionAnimation extends Animation {
	
	private static final float[] COLOR0 = Util.intColorToFloat(0xFACA0C);
	private static final float[] COLOR1 = Util.intColorToFloat(0xFA440C);
	
	protected SoundPool soundPool;
	
	protected GLMatrixStack mStack;
	
	protected Vec2f position;
	
	protected Vec4f properties;
	
	protected GLVertexBufferObject vboPlane;
	
	protected int prgExplosion;
	
	protected int sndExplosion;
	
	public ExplosionAnimation(Vec2f position) {
		this(position, 1.0f, PRNG.nextFloat(3, 6));
	}
	
	public ExplosionAnimation(Vec2f position, float timeDecay, float growRate) {
		this.mStack = new GLMatrixStack(1);
		this.position = position;
		this.properties = new Vec4f();
		this.vboPlane = ResourceLoader.getCommmonVBO(Constants.VBO_PLANE);
		this.prgExplosion = ResourceLoader.getProgram(Constants.PRG_ANIM_EXPLOSION);
		this.soundPool = ResourceLoader.getSoundPool();
		this.sndExplosion = ResourceLoader.getSoundID(Constants.SND_DISTANT_EXPLOSION);
		
		properties.setComponent(0, 0); // time
		properties.setComponent(1, timeDecay); // decay
		properties.setComponent(2, growRate); // grow rate
	}
	


	@Override
	public void onStart() {
		soundPool.play(sndExplosion, 0.5f, 0.5f, 1, 0, 1.0f);
	}

	@Override
	public void onUpdate(float dt) {
		if(getTime() < getTimeDecay()) {
			
			setTime(getTime() + dt);
			
			if(getTime() > getTimeDecay())
				setTime(getTimeDecay());
		
		} else
			stop();
	}

	@Override
	public void onDraw(float[] vpMatrix) {
		
		int hPosition, hTexCoord, hMVP, hProperties, hColor0, hColor1;
		
		mStack.set(vpMatrix);
		mStack.translate(position.getX(), position.getY(), 0);
		
		hMVP = GLES20.glGetUniformLocation(prgExplosion, "in_MVP");
		hProperties = GLES20.glGetUniformLocation(prgExplosion, "in_Properties");
		hColor0 = GLES20.glGetUniformLocation(prgExplosion, "in_Color0");
		hColor1 = GLES20.glGetUniformLocation(prgExplosion, "in_Color1");
		
		hPosition = GLES20.glGetAttribLocation(prgExplosion, "in_Position");
		hTexCoord = GLES20.glGetAttribLocation(prgExplosion, "in_TexCoord");
		
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glUseProgram(prgExplosion);
		
			vboPlane.bind();
		
				GLES20.glUniformMatrix4fv(hMVP, 1, false, mStack.current(), 0);
				GLES20.glUniform4fv(hProperties, 1, properties.getComponents(), 0);
				GLES20.glUniform3fv(hColor0, 1, COLOR0, 0);
				GLES20.glUniform3fv(hColor1, 1, COLOR1, 0);
				
				GLES20.glEnableVertexAttribArray(hPosition);
				GLES20.glEnableVertexAttribArray(hTexCoord);
				
				GLES20.glVertexAttribPointer(hPosition, 3, GLES20.GL_FLOAT, false, 32, 0);
				GLES20.glVertexAttribPointer(hTexCoord, 2, GLES20.GL_FLOAT, false, 32, 24);
				
				GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vboPlane.getVertexCount());
				
				GLES20.glDisableVertexAttribArray(hPosition);
				GLES20.glDisableVertexAttribArray(hTexCoord);
			vboPlane.unbind();
		
		GLES20.glUseProgram(0);
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		GLES20.glDisable(GLES20.GL_BLEND);
	}

	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		
	}

	protected float getTime() { return properties.getComponent(0); }
	protected float getTimeDecay()  { return properties.getComponent(1); }
	protected float getGrowRate() { return properties.getComponent(2); }
	
	protected void setTime(float time) { this.properties.setComponent(0, time); }
	
}
