/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.protocol;

import andrea.bucaletti.cardgames.cards.Card;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
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

    public void writeInt(int x) throws IOException {
        out.writeInt(x);
    }

    public void writeOk() throws IOException {
        out.writeInt(Protocol.M_OK);
    }

    public void writeErr(String description) throws IOException {
        out.writeInt(Protocol.M_ERR);
        out.writeUTF(description);
    }

    public String readUTF() throws IOException {
        return in.readUTF();
    }

    public void writeUTF(String str) throws IOException {
        out.writeUTF(str);
    }

    public DataSocket.Response readResponse() throws IOException, ProtocolException {
        return DataSocket.Response.readFromDataSocket(this);
    }

    public void writeCommand(int cmd) throws IOException {
        out.writeInt(Protocol.M_CMD);
        out.writeInt(cmd);
    }

    public void writeCard(Card card) throws IOException {
        out.writeInt(card.getSuit());
        out.writeInt(card.getRank());
    }

    public Card readCard() throws IOException, ProtocolException {
        int suit = in.readInt();
        int rank = in.readInt();
        return new Card(suit, rank);
    }

    public void readAck() throws ProtocolException, IOException {
        int m = in.readInt();
        if (m != Protocol.M_ACK) {
            throw new ProtocolException("Expected ACK (" + Protocol.M_ACK + "), got " + m);
        }
    }

    public void writeAck() throws IOException {
        out.writeInt(Protocol.M_ACK);
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

            if (resp == Protocol.M_OK) {
                ok = true;
            } else if (resp == Protocol.M_ERR) {
                ok = false;
                errDesc = socket.readUTF();
            } else {
                throw new ProtocolException("Expected OK(" + Protocol.M_OK + ") or ERR(" + Protocol.M_ERR + "). Found: " + resp);
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

        public void assertOk() throws CommandException {
            if (!ok) {
                throw new CommandException(errorDescription);
            }
        }
    }
}
