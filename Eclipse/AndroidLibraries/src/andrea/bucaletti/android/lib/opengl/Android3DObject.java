package andrea.bucaletti.android.lib.opengl;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

import andrea.bucaletti.android.lib.io.EndianDataInputStream;
import android.util.Log;

public class Android3DObject {
	
	private int version;
	private int shadingGroupCount;
	private ArrayList<ShadingGroup> shadingGroups;
	
	protected Android3DObject() {
		version = 0;
		shadingGroupCount = 0;
		shadingGroups = new ArrayList<ShadingGroup>();
	}
	
	public static Android3DObject Load(InputStream in) throws IOException {
		EndianDataInputStream din = new EndianDataInputStream(in, ByteOrder.LITTLE_ENDIAN);
		Android3DObject result = new Android3DObject();
		
		result.version = din.readInt();
		result.shadingGroupCount = din.readInt();
		
		Log.d("Test3", "Version: " + result.version + " Shading groups: " + result.shadingGroupCount);
		
		for(int i = 0; i < result.shadingGroupCount; i++) {
			int idLen, vertexCount;
			byte[] idData;
			float[] vertices;
			String id;
			
			idLen = din.readInt();
			idData = new byte[idLen];
			din.read(idData, 0, idLen);
			id = new String(idData, Charset.forName("US-ASCII"));
			vertexCount = din.readInt();
			
			Log.d("Test3", "Shading group: " + id + "(vertices: " + vertexCount + ")");
			
			vertices = new float[vertexCount * 8]; // 8 floats per vertex (x, y, z, nx, ny, nz, tu, tv)		
			
			if(vertexCount == 0)
				continue;			
			
			
			for(int j = 0; j < vertexCount * 8; j++) {
				vertices[j] = din.readFloat();
				//Log.d("Test3", (j/8) + ": " + vertices[j] + " RemainingBytes: " + din.available());
			}
			
			result.shadingGroups.add(new ShadingGroup(id, vertexCount, vertices));	
			
		}
		
		return result;
	}
	
	public int getVersion() {
		return version;
	}
	
	public int getShadingGroupCount() {
		return shadingGroupCount;
	}
	
	public ShadingGroup getShadingGroup(String id) {
		for(int i = 0; i< shadingGroups.size(); i++)
			if(shadingGroups.get(i).getID().equals(id))
				return shadingGroups.get(i);
		
		return null;
	}
	
	public List<ShadingGroup> getShadingGroups() {
		return shadingGroups;
	}
	
	public static class ShadingGroup {
		private String id;
		private int vertexCount;
		private FloatBuffer vertices;
		
		public ShadingGroup(String id, int verticesCount, float[] data) {
			this.id = id;
			this.vertexCount = verticesCount;
			this.vertices = GLU.createFloatBuffer(data);
		}
		
		public String getID() {
			return id;
		}
		
		public int getVertexCount() {
			return vertexCount;
		}
		
		public FloatBuffer getVertexData() {
			return vertices;
		}
		
		public GLVertexBufferObject asVertexBuffer(int usage) {
			GLVertexBufferObject vbo = new GLVertexBufferObject(vertices, usage);
			vbo.setVertexCount(vertexCount);
			return vbo;
		}
	}
}
