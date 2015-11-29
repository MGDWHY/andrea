package com.erbuka.fireworks;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.Iterator;
import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.util.Log;

public class FireworksRenderer implements GLSurfaceView.Renderer {
	
	private final String vsBgCode =
			"attribute vec2 in_TexCoord;\n"+
			"varying vec2 fs_TexCoord;\n"+
			"void main() {\n" +
				"gl_Position = vec4(in_TexCoord.x * 2.0 - 1.0, in_TexCoord.y * 2.0 - 1.0, 0, 1);\n" +
				"fs_TexCoord = in_TexCoord;\n"+
			"}";
	
	private final String fsBgCode =
			"uniform sampler2D in_Texture;\n"+
			"varying vec2 fs_TexCoord;\n"+
			"void main() {\n"+
				"gl_FragColor = vec4(texture2D(in_Texture, fs_TexCoord).rgb, 1.0);\n"+
			"}";

	
	private final String vsFireworkCode =
			"uniform mat4 in_ProjectionMatrix;\n"+
	
			"attribute float in_Size;\n"+
			"attribute vec3 in_Color;\n"+
			"attribute vec2 in_Position;\n" +
			"attribute float in_Alpha;\n"+
			
			"varying vec3 fs_Color;\n"+
			"varying float fs_Alpha;\n"+
			
			"void main() { \n" +
				"gl_Position = in_ProjectionMatrix * vec4(in_Position.x, in_Position.y, 0, 1);\n" +
				"gl_PointSize = in_Size;\n" +
				"fs_Alpha = in_Alpha;\n"+
				"fs_Color = in_Color;\n"+
			"}";
	
	private final String fsFireworkCode =
			"precision mediump float;\n"+
			"varying vec3 fs_Color;\n"+
			"varying float fs_Alpha;\n"+	
			"void main() { \n" +
				"gl_FragColor = vec4(fs_Color, fs_Alpha);\n" +
			"}";
	
	private static final int[] COLORS = {
		Color.RED, Color.BLUE, Color.GREEN,
		Color.YELLOW, Color.MAGENTA, Color.CYAN,
		Color.WHITE
	};
	
	private FireworkFactory[] factories;
	
	private Vector<ByteBuffer> fireworks;
	
	private int bgProgram, fwProgram;
	
	private float width, height;
	
	private float[] projectionMatrix;
	
	private int bgTextureID;
	
	private FloatBuffer bgVertexBuffer;
	private ShortBuffer bgIndexBuffer;
	
	long prevTime = -1;
	
	private Context context;
	
	public FireworksRenderer(Context context) {
		this.context = context;
	}
	
	public void addFirework(float x, float y, float timePressed) {
		
		int factory = (int) (Math.random() * factories.length);
		
		FireworkFactory.Params params = new FireworkFactory.Params();
		
		params.x = x;
		params.y = height - y;
		params.timePressed = timePressed;
		params.color0 = COLORS[(int)(Math.random() * COLORS.length)];
		params.color1 = COLORS[(int)(Math.random() * COLORS.length)];
		
		ByteBuffer fw = factories[factory].generate(params);
		
		synchronized (this) {	
			fireworks.add(fw);
		}
	}
	
	
	public void onDrawFrame(GL10 gl) {
		if(prevTime == -1)
			prevTime = System.currentTimeMillis();
		
		float dt = (System.currentTimeMillis() - prevTime) / 1000.0f;
		prevTime = System.currentTimeMillis();
		
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
		
		// draw background
		
		GLES20.glUseProgram(bgProgram);
		checkGLError("glUseProgram bgProgram");
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, bgTextureID);
		
		int texCoordHandler = GLES20.glGetAttribLocation(bgProgram, "in_TexCoord");
		int textureHandler = GLES20.glGetUniformLocation(bgProgram, "in_Texture");
		
		GLES20.glUniform1i(textureHandler, 0);
		
		GLES20.glEnableVertexAttribArray(texCoordHandler);
		
		GLES20.glVertexAttribPointer(texCoordHandler, 2, GLES20.GL_FLOAT, false, 8, bgVertexBuffer);
		
		GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, bgIndexBuffer);
		
		GLES20.glDisableVertexAttribArray(texCoordHandler);
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		GLES20.glUseProgram(0);
		
		// draw fireworks
		GLES20.glUseProgram(fwProgram);
		checkGLError("glUseProgram fwProgram");
		
		int projHandler = GLES20.glGetUniformLocation(fwProgram, "in_ProjectionMatrix");
		GLES20.glUniformMatrix4fv(projHandler, 1, false, projectionMatrix, 0);
		
		synchronized(this) {
			Iterator<ByteBuffer> it = fireworks.iterator();
			while(it.hasNext()) {
				ByteBuffer fw = it.next();
				
				if(SparkManager.updateSparkBuffer(fw, dt))
					drawFirework(fw);
				else
					it.remove();
			}
		}
		
		GLES20.glUseProgram(0);
	}
	
	public void drawFirework(ByteBuffer fw) {
		
		fw.position(0);
		
		int positionHandler = GLES20.glGetAttribLocation(fwProgram, "in_Position");
		int colorHandler = GLES20.glGetAttribLocation(fwProgram, "in_Color");
		int alphaHandler = GLES20.glGetAttribLocation(fwProgram, "in_Alpha");
		int sizeHandler = GLES20.glGetAttribLocation(fwProgram, "in_Size");
		
		GLES20.glEnableVertexAttribArray(positionHandler);
		GLES20.glEnableVertexAttribArray(colorHandler);
		GLES20.glEnableVertexAttribArray(alphaHandler);
		GLES20.glEnableVertexAttribArray(sizeHandler);
		
		// set up position attribute ( 8 bytes )
		fw.position(SparkManager.POSITION_OFFSET);
		GLES20.glVertexAttribPointer(positionHandler, 2, GLES20.GL_FLOAT, false, SparkManager.SPARK_SIZE_IN_BYTES, fw);
		
		// set up size attribute ( 4 bytes )
		fw.position(SparkManager.SIZE_OFFSET);
		GLES20.glVertexAttribPointer(sizeHandler, 1, GLES20.GL_FLOAT, false, SparkManager.SPARK_SIZE_IN_BYTES, fw);		

		// set up alpha (time) attribute ( 4 bytes )
		fw.position(SparkManager.ALPHA_OFFSET);
		GLES20.glVertexAttribPointer(alphaHandler, 1, GLES20.GL_FLOAT, false, SparkManager.SPARK_SIZE_IN_BYTES, fw);
		
		// set up colour attribute ( 12 bytes )
		fw.position(SparkManager.COLOR_OFFSET);
		GLES20.glVertexAttribPointer(colorHandler, 3, GLES20.GL_FLOAT, false, SparkManager.SPARK_SIZE_IN_BYTES, fw);		
		
		GLES20.glDrawArrays(GLES20.GL_POINTS, 0, fw.limit() / SparkManager.SPARK_SIZE_IN_BYTES);
		
		GLES20.glDisableVertexAttribArray(positionHandler);	
		GLES20.glDisableVertexAttribArray(colorHandler);	
		GLES20.glDisableVertexAttribArray(alphaHandler);	
		GLES20.glDisableVertexAttribArray(sizeHandler);

	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		GLES20.glViewport(0, 0, width, height);
		
		Matrix.setIdentityM(projectionMatrix, 0);
		Matrix.orthoM(projectionMatrix, 0, 0, width, 0, height, -1, 1);
	
		this.width = width;
		this.height = height;		
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		
		// pepare gl
		
		GLES20.glClearColor(0, 0, 0, 1);
		GLES20.glDisable(GLES20.GL_CULL_FACE);
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE);
		
		// shaders
		
		fwProgram = createProgram(vsFireworkCode, fsFireworkCode);
		bgProgram = createProgram(vsBgCode, fsBgCode);
		
		// matrices and view
		
		projectionMatrix = new float[16];
		
		width = 0;
		height = 0;
		
		// sparks vector
		
		fireworks = new Vector<ByteBuffer>();
		
		// Firework factories
		
		factories = new FireworkFactory[2];
		
		factories[0] = new NormalFireworkFactory();
		factories[1] = new FountainFirewokFactory();
		
		// vertex buffers
		
		float[] coords = { // u, v	
			0, 0, // bottom left
			1, 0, // bottom right,
			0, 1, // top left
			1, 1, // top right
		};
		
		short[] indexes = { 0, 1, 2, 1, 3, 2 };
		
		ByteBuffer bbv = ByteBuffer.allocateDirect(coords.length * 4);
		bbv.order(ByteOrder.nativeOrder());
		bgVertexBuffer = bbv.asFloatBuffer();
		bgVertexBuffer.put(coords);
		bgVertexBuffer.position(0);
		
		ByteBuffer bbi = ByteBuffer.allocateDirect(indexes.length * 2);
		bbi.order(ByteOrder.nativeOrder());
		bgIndexBuffer = bbi.asShortBuffer();
		bgIndexBuffer.put(indexes);
		bgIndexBuffer.position(0);

		// Textures
		
		int[] temp = new int[1];
		GLES20.glGenTextures(1, temp, 0);
		bgTextureID = temp[0];
	
		Bitmap bmp = BitmapFactory.decodeResource(context.getResources(), R.raw.nightsky);		
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, bgTextureID);
		
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
	
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0);
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		bmp.recycle();
	}
	
	private static void checkGLError(String tag) {
		int error = GLES20.glGetError();
		
		if(error != GLES20.GL_NO_ERROR)
			Log.e(tag, "GL Error: " + error);
	}
	
    private int loadShader(int shaderType, String source) {
        int shader = GLES20.glCreateShader(shaderType);
        if (shader != 0) {
            GLES20.glShaderSource(shader, source);
            GLES20.glCompileShader(shader);
            int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                Log.e("loadShader", "Could not compile shader " + shaderType + ":");
                Log.e("loadShader", GLES20.glGetShaderInfoLog(shader));
                GLES20.glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }

    private int createProgram(String vertexSource, String fragmentSource) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return 0;
        }

        int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) {
            return 0;
        }

        int program = GLES20.glCreateProgram();
        if (program != 0) {
            GLES20.glAttachShader(program, vertexShader);
            checkGLError("glAttachShader");
            GLES20.glAttachShader(program, pixelShader);
            checkGLError("glAttachShader");
            GLES20.glLinkProgram(program);
            int[] linkStatus = new int[1];
            GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
            if (linkStatus[0] != GLES20.GL_TRUE) {
                Log.e("createProgram", "Could not link program: ");
                Log.e("createProgrem", GLES20.glGetProgramInfoLog(program));
                GLES20.glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }

}
