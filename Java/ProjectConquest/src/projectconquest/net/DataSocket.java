/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 *
 * @author Andrea
 */
public class DataSocket {

    public Socket socket;
    public DataInputStream in;
    public DataOutputStream out;

    public DataSocket(Socket socket) throws IOException {
        this.socket = socket;

        openStreams();
    }

    public void flushInputStream() {
        try {
            while (in.available() > 0) {
                in.skip(in.available());
            }
        } catch (IOException ex) {
        }
    }

    public int readInt() throws IOException {
        return in.readInt();
    }
    
    public int assertInt(int x) throws IOException, ProtocolException {
        int y = in.readInt();
        if(y != x)
            throw new ProtocolException("Assert " + x + " failed: found " + y);
        
        return y;
    }

    public void writeInt(int x) throws IOException {
        out.writeInt(x);
    }

    public String readUTF() throws IOException {
        return in.readUTF();
    }

    public void writeUTF(String str) throws IOException {
        out.writeUTF(str);
    }

    /**
     * Write game server command
     * @param cmd
     * @throws IOException 
     */
    public void writeGSCommand(int cmd) throws IOException { 
        out.writeInt(Protocol.M_GS_CMD);
        out.writeInt(cmd);
    }
    
    public void assertGSCommand(int cmd) throws IOException, ProtocolException {
        try {
            assertInt(Protocol.M_GS_CMD);
            assertInt(cmd);
        }
        catch(ProtocolException ex) {
            throw new IOException("Assert GameServer Command failed: " + cmd);
        }
    }
    
    public int readGSCommand() throws IOException, ProtocolException {
        try {
            assertInt(Protocol.M_GS_CMD);
            return readInt();
        }
        catch(ProtocolException ex) {
            throw new IOException("Read GameServer Command failed");
        }
    }

    public void writeAck() throws IOException {
        out.writeInt(Protocol.M_ACK);
    }
    
    public void writeNack(String desc) throws IOException {
        out.writeInt(Protocol.M_NACK);
        out.writeUTF(desc);
    }    
    
    public Response readResponse() throws IOException, ProtocolException {
        return Response.readFromDataSocket(this);
    }
    
    public void close() throws IOException {
        socket.close();
    }    

    private void openStreams() throws IOException {
        this.in = new DataInputStream(socket.getInputStream());

        this.out = new DataOutputStream(socket.getOutputStream());
    }
    
    public static class Response {

        private boolean ok;
        private String errorDescription;

        public static Response readFromDataSocket(DataSocket socket) throws IOException, ProtocolException {
            int resp = socket.readInt();

            boolean ok;
            String errDesc = null;

            if (resp == Protocol.M_ACK) {
                ok = true;
            } else if (resp == Protocol.M_NACK) {
                ok = false;
                errDesc = socket.readUTF();
            } else {
                throw new ProtocolException("Expected ACK(" + Protocol.M_ACK + ") or NACK(" + Protocol.M_NACK + "). Found: " + resp);
            }

            return new Response(ok, errDesc);
        }

        private Response(boolean ok, String errorDescription) {
            this.ok = ok;
            this.errorDescription = errorDescription;
        }

        public boolean isOk() {
            return ok;
        }

        public boolean isError() {
            return !ok;
        }

        public String getErrorDescription() {
            return errorDescription;
        }

        public void assertOk() throws ProtocolException {
            if (!ok) {
                throw new ProtocolException(errorDescription);
            }
        }
    }    
    
}
