package andrea.bucaletti.android.darts.tests;

import java.io.IOException;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import andrea.bucaletti.android.darts.gui.GameUI;
import andrea.bucaletti.android.darts.lib.Projectile;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.Timer;
import andrea.bucaletti.android.lib.opengl.Android3DObject;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.android.lib.motion.RotationDetector;
import andrea.bucaletti.android.lib.motion.SimpleRotationDetector;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import andrea.bucaletti.darts.R;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.Bundle;

public class Test3 extends Activity {
	
	private static String vertexShader =
			"uniform mat4 in_MVP;"+
			"attribute vec3 in_Position;" +
			"attribute vec2 in_TexCoord;"+
			"varying vec2 fs_TexCoord;"+
			"void main() {"+
				"gl_Position = in_MVP * vec4(in_Position, 1);"+
				"fs_TexCoord = in_TexCoord;"+
			"}";
	
	private static String fragmentShader =
			"precision mediump float;"+
			"uniform vec4 in_Color;"+
			"uniform sampler2D in_Texture;"+
			"varying vec2 fs_TexCoord;"+
			"void main() {"+
				"gl_FragColor = texture2D(in_Texture, fs_TexCoord);"+
			"}";	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(new MyGLSurfaceView(getApplicationContext()));
	}
	
	private class MyGLSurfaceView extends GLSurfaceView {	
		public MyGLSurfaceView(Context context) {
			super(getApplicationContext());
			
			SimpleRotationDetector rotationDetector = new SimpleRotationDetector(getApplicationContext());
			GameUI gameUI = new GameUI(context);
			rotationDetector.calibrate(64);
			
			setOnTouchListener(gameUI);
			setEGLContextClientVersion(2);
			setRenderer(new MyRenderer(context, rotationDetector, gameUI));
			setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);			
		}
	}
	
	private class MyRenderer implements GLSurfaceView.Renderer, GameUI.Listener {
		
		private Context context;
		private int program, axesProgram, g3dPhongProgram;
		private int targetTextureID;
		private int refTexID;
		private float color[] = { 1, 0, 0, 1 }; 
		
		private GLMatrixStack projStack, modelViewStack;
		
		private float mvpMatrix[] = new float[16];
		
		private RotationDetector rotationDetector;
		private Android3DObject a3o;
		private GLVertexBufferObject vbo;
		private GameUI gameUI;
		private Timer timer;
		private Vec3f position = new Vec3f(0, 0, 10);
		private Vec3f targetPosition = new Vec3f(0, 5, -5);
		
		private Projectile projectile;
		private Axes axes;
		
		public MyRenderer(Context context, RotationDetector detector, GameUI gameUI) {
			this.rotationDetector = detector;
			this.context = context;
			this.gameUI = gameUI;
			this.timer = new Timer();
			this.projStack = new GLMatrixStack(4);
			this.modelViewStack = new GLMatrixStack(16);
			
			try {
				this.a3o = Android3DObject.Load(context.getResources().openRawResource(R.raw.sphere));
			}
			catch(IOException ex) {
				ex.printStackTrace();
				System.exit(-1);
			}
		}
		
		public void Shoot(float force) {
			
			float[] sdir = {0, 0, -1, 0 };
			float[] matrix = new float[16];
			float[] fdir = new float[4];
			
			Matrix.setIdentityM(matrix, 0);
			
			Matrix.rotateM(matrix, 0, rotationDetector.getRotateX(), 1, 0, 0);					
			Matrix.rotateM(matrix, 0, rotationDetector.getRotateY(), 0, 1, 0);
			Matrix.rotateM(matrix, 0, rotationDetector.getRotateZ(), 0, 0, 1);	
			
			Matrix.multiplyMV(fdir, 0, matrix, 0, sdir, 0);
			
			Projectile.Parameters params = new Projectile.Parameters();
			Projectile.Modifiers mods = new Projectile.Modifiers();
			
			params.StartPosition = new Vec3f(position);
			params.Direction = new Vec3f(fdir[0], fdir[1], fdir[2]).normalize();
			params.Force = force;
			params.WindDirection = new Vec3f();
			params.WindForce = 0;
			
			mods.TimeFactor = 0.1f;
			
			projectile = new Projectile(params, mods);
		}
		
		@Override
		public void onDrawFrame(GL10 gl) {
			
			int positionHandle, normalHandle, texCoordHandle, mvpHandle, colorHandle;
			float[] matTemp = new float[16];
			FloatBuffer vertices;
			
			float dt = timer.dt();
			
			gameUI.update(dt);
			
			if(projectile != null) {
				projectile.update(dt);
				position = projectile.getPosition();
				
				if(position.z < 0) {
					projectile = null;
					position = new Vec3f(0, 0, 10);
					gameUI.setReady();
				}
				
			}
			
			vertices = a3o.getShadingGroups().get(0).getVertexData();
			
			modelViewStack.setIdentity();
			modelViewStack.rotate(-rotationDetector.getRotateZ(), 0, 0, 1);
			modelViewStack.rotate(-rotationDetector.getRotateY(), 0, 1, 0);
			modelViewStack.rotate(-rotationDetector.getRotateX(), 1, 0, 0);
			modelViewStack.translate(-position.x, -position.y, -position.z);
			
			Matrix.multiplyMM(mvpMatrix, 0, projStack.current(), 0, modelViewStack.current(), 0);
			
			GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
			
			GLES20.glEnable(GLES20.GL_DEPTH_TEST);
			GLES20.glDisable(GLES20.GL_BLEND);
			
			GLES20.glUseProgram(g3dPhongProgram);
			
				mvpHandle = GLES20.glGetUniformLocation(g3dPhongProgram, "in_MVPMatrix");
				colorHandle = GLES20.glGetUniformLocation(g3dPhongProgram, "in_Color");
				
				texCoordHandle = GLES20.glGetAttribLocation(g3dPhongProgram, "in_TexCoord");
				positionHandle = GLES20.glGetAttribLocation(g3dPhongProgram, "in_Position");
				normalHandle = GLES20.glGetAttribLocation(g3dPhongProgram, "in_Normal");
				
				GLES20.glUniform4fv(colorHandle, 1, color, 0);
				GLES20.glUniformMatrix4fv(mvpHandle, 1, false, mvpMatrix, 0);
				
				vbo.bind();
						
				GLES20.glEnableVertexAttribArray(positionHandle);
				GLES20.glEnableVertexAttribArray(normalHandle);
				GLES20.glEnableVertexAttribArray(texCoordHandle);
				
				GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 32, 0);
				GLES20.glVertexAttribPointer(normalHandle, 3, GLES20.GL_FLOAT, false, 32, 12);
				GLES20.glVertexAttribPointer(texCoordHandle, 2, GLES20.GL_FLOAT, false, 32, 24);
				
				GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, a3o.getShadingGroups().get(0).getVertexCount());
				
				GLU.openGLError("Error while binding vertex buffer");
				
				GLES20.glDisableVertexAttribArray(normalHandle);
				GLES20.glDisableVertexAttribArray(positionHandle);
				GLES20.glDisableVertexAttribArray(texCoordHandle);
				
				vbo.unbind();
			
			GLES20.glUseProgram(0);
			
			GLES20.glUseProgram(axesProgram);
				mvpHandle = GLES20.glGetUniformLocation(axesProgram, "in_MVP");
				colorHandle = GLES20.glGetUniformLocation(axesProgram, "in_Color");
				positionHandle = GLES20.glGetAttribLocation(axesProgram, "in_Position");
				
				GLES20.glUniformMatrix4fv(mvpHandle, 1, false, mvpMatrix, 0);
				
				axes.draw(positionHandle, colorHandle);
				
			GLES20.glUseProgram(0);
			
			gameUI.draw();
			
			GLU.openGLError("Error at end of draw");
			
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GLES20.glViewport(0, 0, width, height);
			
			projStack.setPerspective(45, (float) width / height, 0.1f, 20);
			
			gameUI.setViewSize(width, height);
			
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
			GLES20.glEnable(GLES20.GL_DEPTH_TEST);
			
			timer.reset();
			gameUI.initilize();
			gameUI.setListener(this);
			
			targetTextureID = GLU.loadTexture2D(R.drawable.target0, getResources());
			
			program = GLU.loadProgram(vertexShader, fragmentShader);
			axesProgram = GLU.loadProgram(R.string.axes_vs, R.string.axes_fs, context.getResources());
			g3dPhongProgram = GLU.loadProgram(R.string.g3d_phong_vs, R.string.g3d_phong_color_fs, context.getResources());
			
			this.vbo = a3o.getShadingGroups().get(0).asVertexBuffer(GLES20.GL_STATIC_DRAW);
			
			axes = new Axes();
		}
		
	}
	
	private class Axes {
		private GLVertexBufferObject buf;
		
		private FloatBuffer position;
		
		private float[] cx = {1, 0, 0, 1};
		private float[] cy = {0, 1, 0, 1};
		private float[] cz = {0, 0, 1, 1};		
		
		public Axes() {
			float p[] = {
					-1, -1, -1,
					9, -1, -1,
					-1, -1, -1,
					-1, 9, -1,
					-1, -1, -1,
					-1, -1, 9
			};
			
			position = GLU.createFloatBuffer(p);
			
			buf = new GLVertexBufferObject(position, GLES20.GL_STATIC_DRAW);
		}
		
		public void draw(int positionHandle, int colorHandle) {
			
			GLES20.glLineWidth(5);
			
			buf.bind();
			
			GLES20.glEnableVertexAttribArray(positionHandle);
			
			GLES20.glVertexAttribPointer(positionHandle,3, GLES20.GL_FLOAT, false, 0, 0);
			
			GLES20.glUniform4fv(colorHandle, 1, cx, 0);
			GLES20.glDrawArrays(GLES20.GL_LINES, 0, 2);

			GLES20.glUniform4fv(colorHandle, 1, cy, 0);
			GLES20.glDrawArrays(GLES20.GL_LINES, 2, 2);
			
			GLES20.glUniform4fv(colorHandle, 1, cz, 0);
			GLES20.glDrawArrays(GLES20.GL_LINES, 4, 2);	
			
			GLES20.glDisableVertexAttribArray(0);
			
			buf.unbind();
			
			GLU.openGLError("Error drawing axes");
			
			
			
		}
	}

}
