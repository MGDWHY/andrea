package projectcolossus.graphics;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import andrea.bucaletti.android.lib.Timer;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.opengl.Android3DObject;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.projectcolossus.R;
import android.content.res.Resources;
import android.opengl.GLES20;
import android.opengl.Matrix;
import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.PlanetBuff;
import projectcolossus.gamelogic.Player;
import projectcolossus.graphics.fx.PlanetFX;
import projectcolossus.graphics.fx.PlanetaryCageFX;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.PRNG;

/* TODO Sistemare meglio le moltiplicazioni tra matrici
 * usando direttamente lo stack e non la classe Matrix
*/
public class PlanetRenderer {
	
	// colors
	private static final float[] AURA_COLOR_PLANET_UNKNOWN = new float[]
			{ 0.0f, 0.5f, 1.0f };
	
	private static final float[] NO_LIGHT_COLOR = new float[]
			{ 1, 1, 1 };
	
	private Timer timer;
	
	// game data
	private GameData gameData;
	
	// game map
	private GameMap gameMap;
	
	// visible planets
	private List<Planet> visiblePlanets;
	
	// matrices
	private GLMatrixStack modelStack;
	
	// programs
	private int prgPlanet;
	private int prgPlanetAura;
	private int prgUnitsOnPlanet;
	
	// vbos
	private GLVertexBufferObject vboPlanet, vboPlanetAura, vboUnitsOnPlanet;
	
	// textures
	private int texPlanetAuraMask;
	private int texUnitsOnPlanet;
	private int texJuppiter; //temp;
	
	public PlanetRenderer(Resources resources, GameMap gameMap) {
		
		this.gameMap = gameMap;
		//this.planetInfoVBO = new HashMap<Integer, GLVertexBufferObject>();
		this.visiblePlanets = new ArrayList<Planet>();
		this.timer = new Timer();

		vboPlanet = ResourceLoader.getCommmonVBO(Constants.VBO_SPHERE);
		vboPlanetAura = ResourceLoader.getCommmonVBO(Constants.VBO_PLANET_AURA_PLANE);
		vboUnitsOnPlanet = ResourceLoader.getCommmonVBO(Constants.VBO_PLANE);
		
		prgPlanet = ResourceLoader.getProgram(Constants.PRG_PLANET);
		prgPlanetAura = ResourceLoader.getProgram(Constants.PRG_PLANET_AURA);
		prgUnitsOnPlanet = ResourceLoader.getProgram(Constants.PRG_UNITS_ON_PLANET);
		
		texPlanetAuraMask = ResourceLoader.getTexture(Constants.TEX_PLANET_AURA);
		texJuppiter = GLU.loadTexture2D(R.raw.tex_juppiter, resources); // temp
		texUnitsOnPlanet = ResourceLoader.getTexture(Constants.TEX_UNITS_ON_PLANET);
		
		modelStack = new GLMatrixStack(4);		
	}
	
	public void setGameData(GameData gameData) { 
		this.gameData = gameData; 
		this.gameMap = gameData.getGameMap();
	}
	public GameData getGameData() { return this.gameData; }
	
	public void setVisiblePlanets(List<Planet> visiblePlanets) {
		this.visiblePlanets = visiblePlanets;
	}
	
	public void drawPlanets(float[] vpMatrix) {
		
		if(gameData == null)
			return;
		
		int mvpHandle, positionHandle, colorHandle, texCoordHandle, 
			textureHandle, useTextureHandle, normalHandle, hLightColor;
		
		float mvpMatrix[] = new float[16];
		float lightColor[];
		float auraColor[];
		
		float tx, ty, sx, sy, sz;
		
		boolean visible;
		
		for(Planet planet : gameMap.getPlanets()) {
			
			lightColor = NO_LIGHT_COLOR;
			 
			tx = planet.getPosition().getX();
			ty = planet.getPosition().getY();
			sx = sy = sz = planet.getRadius() * 2.0f;			
		
			modelStack.setIdentity();
			modelStack.translate(tx, ty, 0);
			modelStack.scale(sx, sy, sz);
			
			visible = visiblePlanets.contains(planet);
			
			if(visible) {
				if(planet.isOwned()) {
					auraColor = planet.getOwner().getFloatColor();
					lightColor = planet.getOwner().getFloatColor();
				} else
					auraColor = AURA_COLOR_PLANET_UNKNOWN;
			} else {
				auraColor = AURA_COLOR_PLANET_UNKNOWN;
			}			
			
			Matrix.multiplyMM(mvpMatrix, 0, vpMatrix, 0, modelStack.current(), 0);

			GLES20.glUseProgram(prgPlanetAura);	
				
				GLES20.glDisable(GLES20.GL_DEPTH_TEST);
				GLES20.glEnable(GLES20.GL_BLEND);
				GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texPlanetAuraMask);
			
				vboPlanetAura.bind();
	
					mvpHandle = GLES20.glGetUniformLocation(prgPlanetAura, "in_MVP");
					colorHandle = GLES20.glGetUniformLocation(prgPlanetAura, "in_Color");
					textureHandle = GLES20.glGetUniformLocation(prgPlanetAura, "in_AuraTexture");
					
					positionHandle = GLES20.glGetAttribLocation(prgPlanetAura, "in_Position");
					texCoordHandle = GLES20.glGetAttribLocation(prgPlanetAura, "in_TexCoord");
					
					GLES20.glUniformMatrix4fv(mvpHandle, 1, false, mvpMatrix, 0);
					GLES20.glUniform3fv(colorHandle, 1, auraColor, 0);
					GLES20.glUniform1i(textureHandle, 0);
					
					GLES20.glEnableVertexAttribArray(positionHandle);
					GLES20.glEnableVertexAttribArray(texCoordHandle);
					
					GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 32, 0);
					GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 32, 24);
					
					GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vboPlanetAura.getVertexCount());
					
					GLES20.glDisableVertexAttribArray(texCoordHandle);
					GLES20.glDisableVertexAttribArray(positionHandle);
				
				vboPlanetAura.unbind();
				
				GLES20.glDisable(GLES20.GL_BLEND);
				GLES20.glEnable(GLES20.GL_DEPTH_TEST);
			
			GLES20.glUseProgram(0);			
			
			modelStack.push();
				modelStack.rotate(planet.getRotationalSpeed() * timer.time(), 0, 1 , 0);
				
				Matrix.multiplyMM(mvpMatrix, 0, vpMatrix, 0, modelStack.current(), 0);
				/*
				GLES20.glEnable(GLES20.GL_STENCIL_TEST);
				GLES20.glStencilFunc(GLES20.GL_ALWAYS, 1, 0xFF);
				GLES20.glStencilOp(GLES20.GL_REPLACE, GLES20.GL_REPLACE, GLES20.GL_REPLACE);*/
				GLES20.glUseProgram(prgPlanet);
				
					vboPlanet.bind();
					
						GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
						GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, visible ? texJuppiter : 0);
					
						mvpHandle = GLES20.glGetUniformLocation(prgPlanet, "in_MVP");
						useTextureHandle = GLES20.glGetUniformLocation(prgPlanet, "in_UseTexture");
						textureHandle = GLES20.glGetUniformLocation(prgPlanet, "in_Texture");
						hLightColor = GLES20.glGetUniformLocation(prgPlanet, "in_LightColor");
						
						positionHandle = GLES20.glGetAttribLocation(prgPlanet, "in_Position");
						normalHandle = GLES20.glGetAttribLocation(prgPlanet, "in_Normal");
						texCoordHandle = GLES20.glGetAttribLocation(prgPlanet, "in_TexCoord");		
						
						GLES20.glUniformMatrix4fv(mvpHandle, 1, false, mvpMatrix, 0);
						GLES20.glUniform3fv(hLightColor, 1, lightColor, 0);
						GLES20.glUniform1i(useTextureHandle, visible ? 1 : 0);
						GLES20.glUniform1i(textureHandle, 0);	
			
						GLES20.glEnableVertexAttribArray(positionHandle);
						GLES20.glEnableVertexAttribArray(normalHandle);
						GLES20.glEnableVertexAttribArray(texCoordHandle);
						
						GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 32, 0);
						GLES20.glVertexAttribPointer(normalHandle, 3,GLES20.GL_FLOAT, false, 32, 12);
						GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 32, 24);
						
						GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vboPlanet.getVertexCount());
						
						GLES20.glDisableVertexAttribArray(positionHandle);
						GLES20.glDisableVertexAttribArray(normalHandle);
						GLES20.glDisableVertexAttribArray(texCoordHandle);
					
					vboPlanet.unbind();
				
				GLES20.glUseProgram(0);
				GLES20.glDisable(GLES20.GL_STENCIL_TEST);
			
			modelStack.pop();
			
			Matrix.multiplyMM(mvpMatrix, 0, vpMatrix, 0, modelStack.current(), 0);
			
			if(visible) 
				drawBuffs(mvpMatrix, planet);
			
		}
	}
	
	public void drawBuffs(float[] mvpMatrix, Planet planet) {
		
		int frameID = PRNG.nextInt();
		
		for(PlanetBuff b : planet.getBuffs()) {
			PlanetFX fx = ResourceLoader.getPlanetFX(b.getID());
			
			fx.update(frameID);
			
			switch(b.getID()) {
			case Constants.IDB_PLANETARY_CAGE:
				fx.setAttribute(Constants.FXATTRIB_COLOR0, b.getApplier().getFloatColor());
				fx.draw(mvpMatrix, planet);
				break;
			case Constants.IDB_RESOURCE_DEPLETION:
			case Constants.IDB_THERMONUCLEAR_BOMBING:
				fx.setAttribute(Constants.FXATTRIB_COLOR0, b.getApplier().getFloatColor());
				fx.draw(mvpMatrix, planet);
				break;
			default:
				break;
			}
			
		}		
	}
	
	public void drawPlanetIcons(float[] vpMatrix) {
		
		float[] mvpMatrix = new float[16];
		
		int mvpHandle, positionHandle, colorHandle, textureHandle, texCoordHandle;
		
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glDisable(GLES20.GL_CULL_FACE);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glUseProgram(prgUnitsOnPlanet);
		
		mvpHandle = GLES20.glGetUniformLocation(prgUnitsOnPlanet, "in_MVP");
		textureHandle = GLES20.glGetUniformLocation(prgUnitsOnPlanet, "in_Texture");
		colorHandle = GLES20.glGetUniformLocation(prgUnitsOnPlanet, "in_Color");
		
		positionHandle = GLES20.glGetAttribLocation(prgUnitsOnPlanet, "in_Position");
		texCoordHandle = GLES20.glGetAttribLocation(prgUnitsOnPlanet, "in_TexCoord");
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texUnitsOnPlanet);
		
		GLES20.glUniform1i(textureHandle, 0);
		
		vboUnitsOnPlanet.bind();
		
		GLES20.glEnableVertexAttribArray(positionHandle);
		GLES20.glEnableVertexAttribArray(texCoordHandle);
		
		GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 32, 0);
		GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 32, 24);
		
		for(Planet planet : visiblePlanets) {
			int counter = 0;
		
			for(Player player : gameData.getPlayersOnPlanet(planet.getID())) {
			
				modelStack.setIdentity();
				modelStack.translate(
						planet.getPosition().getX() - planet.getRadius() + counter++ * 8, 
						planet.getPosition().getY() + planet.getRadius(), 
						0
				);
				modelStack.rotate(timer.time() * 180, 0, 1, 0);
				modelStack.scale(8, 8, 8);
				
				Matrix.multiplyMM(mvpMatrix, 0, vpMatrix, 0, modelStack.current(), 0);
				
				GLES20.glUniformMatrix4fv(mvpHandle, 1, false, mvpMatrix, 0);
				GLES20.glUniform3fv(colorHandle, 1, player.getFloatColor(), 0);
					
				GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vboUnitsOnPlanet.getVertexCount());
			}
		}
		
		GLES20.glDisableVertexAttribArray(positionHandle);
		GLES20.glDisableVertexAttribArray(colorHandle);		
		
		vboUnitsOnPlanet.unbind();
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		GLES20.glUseProgram(0);
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_CULL_FACE);
		GLES20.glDisable(GLES20.GL_BLEND);
	}
	
}
