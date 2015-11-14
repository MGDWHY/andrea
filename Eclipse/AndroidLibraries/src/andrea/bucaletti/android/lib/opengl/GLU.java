package andrea.bucaletti.android.lib.opengl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

import andrea.bucaletti.android.lib.vecmath.Vec3f;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class GLU {
	
	public static final String GL_ERROR_TAG = "GLError";
	public static final String GL_INFO_TAG = "GLInfo";
	
	public static int loadShader(String shaderCode, int type) {
	    // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
	    // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
		int status[] = new int[1];
	    int shader = GLES20.glCreateShader(type);

	    // add the source code to the shader and compile it
	    GLES20.glShaderSource(shader, shaderCode);
	    GLES20.glCompileShader(shader);
	    
	    GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, status, 0);
	    
	    if(status[0] == GLES20.GL_TRUE) {
	    	Log.d(GL_INFO_TAG, "Shader compiled!");
	    } else {
	    	Log.e(GL_ERROR_TAG, "Shader not compiled!");
	    	Log.e(GL_ERROR_TAG, "Info Log: " + GLES20.glGetShaderInfoLog(shader));
	    }

	    return shader;		
	}
	
	public static int loadProgram(String vertexShader, String fragmentShader) {
		int program = GLES20.glCreateProgram();
		int vs = loadShader(vertexShader, GLES20.GL_VERTEX_SHADER);
		int fs = loadShader(fragmentShader, GLES20.GL_FRAGMENT_SHADER);
		
		GLES20.glAttachShader(program, vs);
		GLES20.glAttachShader(program, fs);
		
		GLES20.glLinkProgram(program);
		
		return program;
	}
	
	public static int loadProgram(int resVertexShader, int resFragmentShader, Resources resources) {
		
		String vsCode = resources.getString(resVertexShader);
		String fsCode = resources.getString(resFragmentShader);
		
		return loadProgram(vsCode, fsCode);
		
	}
	
	public static FloatBuffer createFloatBuffer(float[] elements) {
		FloatBuffer result = ByteBuffer.allocateDirect(elements.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		
		result.put(elements);
		result.position(0);
		
		return result;
		
	}
	
	public static ShortBuffer createShortBuffer(short[] elements) {
		ShortBuffer result = ByteBuffer.allocateDirect(elements.length * 2)
				.order(ByteOrder.nativeOrder()).asShortBuffer();
		
		result.put(elements);
		result.position(0);
		
		return result;
	}	
	
	public static int loadTexture2D(int resId, Resources resources) {
		Bitmap bmp = BitmapFactory.decodeResource(resources, resId);
		int[] texId = new int[1];
		
		GLES20.glGenTextures(1, texId, 0);
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texId[0]);
		
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR_MIPMAP_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0);
		
		GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D);
		
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		bmp.recycle();
		
		return texId[0];
	}
	
	public static void openGLError() {
		int err = GLES20.glGetError();
		if(err != 0)
			Log.e("GLError", GLUtils.getEGLErrorString(err));
	}
	
	public static void openGLError(String desc) {
		int err = GLES20.glGetError();
		if(err != 0)
			Log.e("GLError", desc + ": " + GLUtils.getEGLErrorString(err));
	}	
	
	public float[] getSphericalBillboardMatrix(Vec3f objPos, Vec3f cameraPos, Vec3f worldUp) {
		float[] result = new float[16];
		
		Vec3f auxZ = cameraPos.sub(objPos).normalize();
		Vec3f auxX = worldUp.cross(auxZ).normalize();
		Vec3f auxY = auxZ.cross(auxX);
		
		// column 1
		result[0] = auxX.x;
		result[1] = auxY.x;
		result[2] = auxZ.x;
		result[3] = 0;
		
		// column 2
		result[5] = auxX.y;
		result[6] = auxY.y;
		result[7] = auxZ.y;
		result[8] = 0;

		// column 3
		result[9] = auxX.z;
		result[10] = auxY.z;
		result[11] = auxZ.z;
		result[12] = 0;		

		// column 4
		result[9] = 0;
		result[10] = 0;
		result[11] = 0;
		result[12] = 1;			
		
		
		return result;
	}
}
