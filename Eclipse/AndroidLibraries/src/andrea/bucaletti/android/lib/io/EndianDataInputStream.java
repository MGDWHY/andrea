package andrea.bucaletti.android.lib.io;

import java.io.DataInputStream;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;

public class EndianDataInputStream extends FilterInputStream {
	
	public static final int LITTLE_ENDIAN = 0;
	public static final int BIG_ENDIAN = 1;
	
	private ByteOrder order;
	
	protected DataInputStream din;
	
	public EndianDataInputStream(InputStream in, ByteOrder order) {
		super(in);
		din = new DataInputStream(in);
		this.order = order;
	}
	
	public short readShort() throws IOException {
		if(isLittleEndian())
			return Short.reverseBytes(din.readShort());
		else
			return din.readShort();
	}
	
	public int readInt() throws IOException {
		if(isLittleEndian())
			return Integer.reverseBytes(din.readInt());
		else
			return din.readInt();
	}
	
	public float readFloat() throws IOException {
		if(isLittleEndian()) 
			return Float.intBitsToFloat(Integer.reverseBytes(din.readInt()));
		else
			return din.readFloat();
	}
	
	public long readLong() throws IOException {
		if(isLittleEndian())
			return Long.reverseBytes(din.readLong());
		else
			return din.readLong();
	}
	
	public double readDouble() throws IOException {
		if(isLittleEndian())
			return Double.longBitsToDouble(Long.reverseBytes(din.readLong()));
		else
			return din.readLong();
	}
	
	public boolean isLittleEndian() {
		return order.equals(ByteOrder.LITTLE_ENDIAN);
	}
	
	public boolean isBigEndian() {
		return order.equals(ByteOrder.BIG_ENDIAN);
	}

}
