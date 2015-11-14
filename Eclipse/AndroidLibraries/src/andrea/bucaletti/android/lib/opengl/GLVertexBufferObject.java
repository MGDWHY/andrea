package andrea.bucaletti.android.lib.opengl;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

import android.opengl.GLES20;

public class GLVertexBufferObject {
	
	private int vertexCount;
	private int vboID;
	
	public GLVertexBufferObject() {
		int[] buf = new int[1];
		
		GLES20.glGenBuffers(1, buf, 0);
		
		if(buf[0] == 0)
			GLU.openGLError("Couldn't create a vertex buffer");
		
		this.vboID = buf[0];		
	}
	
	public GLVertexBufferObject(FloatBuffer data, int usage) {
		this(data, 4, usage);
	}
	
	public GLVertexBufferObject(ByteBuffer data, int usage) {
		this(data, 1, usage);
	}
	
	public GLVertexBufferObject(Buffer data, int elementSize, int usage) {
		int[] buf = new int[1];
		
		GLES20.glGenBuffers(1, buf, 0);
		
		if(buf[0] == 0)
			GLU.openGLError("Couldn't create a vertex buffer");
		
		this.vboID = buf[0];
		
		setBufferData(data, elementSize, usage);
	}
	
	public void setBufferData(int sizeInBytes, int usage) {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vboID);
		
		GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, sizeInBytes, null, usage);
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);		
	}
	
	
	public void setBufferData(Buffer data, int elementSize, int usage) {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vboID);
		
		GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, data.capacity() * elementSize, data, usage);
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		
		GLU.openGLError("Error while filling vertex buffer data");
	}
	
	public void setBufferSubData(Buffer data, int elementSize, int offset) {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vboID);
		
		GLES20.glBufferSubData(GLES20.GL_ARRAY_BUFFER, offset, data.capacity() * elementSize, data);
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);	
		
		GLU.openGLError("Error while filling vertex buffer data");
	}
	
	public void setVertexCount(int vertexCount) {
		this.vertexCount = vertexCount;
	}
	
	public int getVertexCount() {
		return vertexCount;
	}
	
	public int getID() {
		return vboID;
	}
	
	public void bind() {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vboID);
	}
	
	public void unbind() {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
	}
	
	public void delete() {
		int[] buf = { vboID };
		GLES20.glDeleteBuffers(1, buf, 0);
	}
}
