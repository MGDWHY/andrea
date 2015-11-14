package projectcolossus.graphics.animation;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import android.opengl.GLES20;
import android.util.Log;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.res.ResourceLoader;

public class MoveUnitAnimation extends Animation {
	
	private static final float TIME = 1.0f;
	
	private float[] color;
	
	private FloatBuffer vertex;
	
	private int prgAnimUnitMove;
	
	private int texUnit;
	
	private float currentTime, alpha;
	
	private Vec3f startPosition, endPosition;
	
	public MoveUnitAnimation(Player player, Planet from, Planet to) {
		this.color = player.getFloatColor();
		this.startPosition = new Vec3f(from.getPosition().getX(), from.getPosition().getY(), 0);
		this.endPosition = new Vec3f(to.getPosition().getX(), to.getPosition().getY(), 0);
		this.vertex = ByteBuffer.allocateDirect(12).order(ByteOrder.nativeOrder()).asFloatBuffer();
		this.prgAnimUnitMove = ResourceLoader.getProgram(Constants.PRG_ANIM_UNIT_MOVE);
		this.texUnit = ResourceLoader.getTexture(Constants.TEX_STAR);
	}

	@Override
	public void onStart() {}

	@Override
	public void onUpdate(float dt) {
		// TODO Auto-generated method stub
		
		if(currentTime < TIME) {
			currentTime += dt;
			
			if(currentTime > TIME)
				currentTime = TIME;
			
			alpha = currentTime / TIME;
			
			Vec3f position = startPosition.scale(1 - alpha).add(endPosition.scale(alpha));
					
			vertex.position(0);
			vertex.put(position.toArray());
			
		} else
			stop();
	}

	@Override
	public void onDraw(float[] vpMatrix) {
		int hMVP, hPosition, hColor, hTexture;
		
		GLES20.glUseProgram(prgAnimUnitMove);
			
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texUnit);
			
			GLES20.glEnable(GLES20.GL_BLEND);
				hMVP = GLES20.glGetUniformLocation(prgAnimUnitMove, "in_MVP");
				hColor = GLES20.glGetUniformLocation(prgAnimUnitMove, "in_Color");
				hTexture = GLES20.glGetUniformLocation(prgAnimUnitMove, "in_Texture");
				
				hPosition = GLES20.glGetAttribLocation(prgAnimUnitMove, "in_Position");
	
				GLES20.glUniformMatrix4fv(hMVP, 1, false, vpMatrix, 0);
				GLES20.glUniform3fv(hColor, 1, color, 0);
				GLES20.glUniform1i(hTexture, 0);
				
				GLU.openGLError();
				
				GLES20.glEnableVertexAttribArray(hPosition);
				
				vertex.position(0);
				GLES20.glVertexAttribPointer(hPosition, 3, GLES20.GL_FLOAT, false, 0, vertex);
				
				GLES20.glDrawArrays(GLES20.GL_POINTS, 0, 1);
				
				GLES20.glDisableVertexAttribArray(hPosition);
				
			GLES20.glDisable(GLES20.GL_BLEND);
			
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
			
		GLES20.glUseProgram(0);
		
		
	}

	@Override
	public void onStop() {}
	
}
