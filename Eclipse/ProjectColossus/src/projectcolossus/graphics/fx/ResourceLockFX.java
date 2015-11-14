package projectcolossus.graphics.fx;

import andrea.bucaletti.android.lib.Timer;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import android.opengl.GLES20;
import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.Util;

public class ResourceLockFX extends PlanetFX {
	
	private Timer timer;
	
	private GLMatrixStack mStack;
	
	private float alpha;
	
	private GLVertexBufferObject vboSphere;
	
	private float[] color;
	
	private int prgResourceLock;
	
	private int tex0, tex1;
	
	public ResourceLockFX(int color) {
		timer = new Timer();
		mStack = new GLMatrixStack(1);
		this.color = Util.intColorToFloat(color);
	}

	@Override
	protected void update() {
		alpha = (float) (Math.sin(timer.time() * 5.0f) + 1.0f) / 2.0f;
	}

	@Override
	public void draw(float[] mvpMatrix, Planet planet) {
		// TODO Auto-generated method stub
		
		int hMVP, hPosition, hTexCoord, hTexLowGlow, hTexHighGlow, hColor, hAlpha;
		
		if(vboSphere == null)
			vboSphere = ResourceLoader.getCommmonVBO(Constants.VBO_SPHERE);
		
		if(prgResourceLock == 0)
			prgResourceLock = ResourceLoader.getProgram(Constants.PRG_FX_RESOURCE_LOCK);
		
		if(tex0 == 0 && tex1 == 0) {
			tex0 = ResourceLoader.getTexture(Constants.TEX_FX_RESOURCE_LOCK_0);
			tex1 = ResourceLoader.getTexture(Constants.TEX_FX_RESOURCE_LOCK_1);
		}
		
		mStack.set(mvpMatrix);
		mStack.rotate(planet.getRotationalSpeed() * timer.time(), 0, 1, 0);
		
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glUseProgram(prgResourceLock);
		
			hMVP = GLES20.glGetUniformLocation(prgResourceLock, "in_MVP");
			hTexLowGlow = GLES20.glGetUniformLocation(prgResourceLock, "in_TexLowGlow");
			hTexHighGlow = GLES20.glGetUniformLocation(prgResourceLock, "in_TexHighGlow");
			hColor = GLES20.glGetUniformLocation(prgResourceLock, "in_Color");
			hAlpha = GLES20.glGetUniformLocation(prgResourceLock, "in_Alpha");
			
			hPosition = GLES20.glGetAttribLocation(prgResourceLock, "in_Position");
			hTexCoord = GLES20.glGetAttribLocation(prgResourceLock, "in_TexCoord");
			
			GLES20.glUniformMatrix4fv(hMVP, 1, false, mStack.current(), 0);
			GLES20.glUniform1i(hTexLowGlow, 0);
			GLES20.glUniform1i(hTexHighGlow, 1);
			GLES20.glUniform3fv(hColor, 1, color, 0);
			GLES20.glUniform1f(hAlpha, alpha);
			
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, tex0);
			
			GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, tex1);
			
			vboSphere.bind();
			
				GLES20.glEnableVertexAttribArray(hPosition);
				GLES20.glEnableVertexAttribArray(hTexCoord);
				
				GLES20.glVertexAttribPointer(hPosition, 3, GLES20.GL_FLOAT, false, 32, 0);
				GLES20.glVertexAttribPointer(hTexCoord, 2, GLES20.GL_FLOAT, false, 32, 24);
				
				GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vboSphere.getVertexCount());
				
				GLES20.glDisableVertexAttribArray(hPosition);
				GLES20.glDisableVertexAttribArray(hTexCoord);
			
			vboSphere.unbind();
		
		GLES20.glUseProgram(0);
		GLES20.glDisable(GLES20.GL_BLEND);
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
	}

}
