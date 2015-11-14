package projectcolossus.graphics;

import java.nio.FloatBuffer;
import java.util.ArrayList;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Route;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import android.content.res.Resources;
import android.opengl.GLES20;


public class RouteRenderer {
	
	// vbos
	private GLVertexBufferObject vboRoutes;
	
	// program
	private int prgRoutes;
	
	public RouteRenderer(Resources resources, GameMap gameMap) {
				
		ArrayList<Route> routes = gameMap.getRoutes();
		float[] vertices = new float[routes.size() * 2 * 3]; // each route has 2 vertices, each vertex 3 coordinates;
		
		
		
		for(int i = 0; i < routes.size(); i++) {
			Planet p0 = routes.get(i).getPlanet(0);
			Planet p1 = routes.get(i).getPlanet(1);
			
			// First vertex
			vertices[6 * i] = p0.getPosition().getX();
			vertices[6 * i + 1] = p0.getPosition().getY();
			vertices[6 * i + 2] = 0;
			
			// Second vertex
			vertices[6 * i + 3] = p1.getPosition().getX();
			vertices[6 * i + 4] = p1.getPosition().getY();
			vertices[6 * i + 5] = 0;			
		}
		
		FloatBuffer buffer = GLU.createFloatBuffer(vertices);
	
		vboRoutes = new GLVertexBufferObject(buffer, GLES20.GL_STATIC_DRAW);	
		vboRoutes.setVertexCount(routes.size() * 2);
		
		prgRoutes = ResourceLoader.getProgram(Constants.PRG_ROUTES);
	}
	
	public void drawRoutes(float[] vpMatrix) {
		
		if(vboRoutes == null)
			return;
		
		int positionHandle, mvpHandle;
		
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
	
		GLES20.glUseProgram(prgRoutes);
		
			vboRoutes.bind();
		
				mvpHandle = GLES20.glGetUniformLocation(prgRoutes, "in_MVP");
				
				positionHandle = GLES20.glGetAttribLocation(prgRoutes, "in_Position");
				
				GLES20.glUniformMatrix4fv(mvpHandle, 1, false, vpMatrix, 0);
				
				GLES20.glEnableVertexAttribArray(positionHandle);
				
				GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 0, 0);
	
				GLES20.glDrawArrays(GLES20.GL_LINES, 0, vboRoutes.getVertexCount());
				
				GLES20.glDisableVertexAttribArray(positionHandle);
				
			vboRoutes.unbind();
		
		GLES20.glUseProgram(0);
		
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
	}
}
