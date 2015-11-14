/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.client;

import andrea.bucaletti.net.game.protocol.DataChannel;
import andrea.bucaletti.net.game.protocol.DataSocket;
import andrea.bucaletti.net.game.protocol.Protocol;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import andrea.bucaletti.cardgames.util.Log;
import andrea.bucaletti.net.game.protocol.CommandException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;

/**
 *
 * @author Andrea
 *
 * Generic Game Client
 */
public abstract class Client extends Thread {

    protected static final String LOG_TAG = "Client";
    protected int port;
    protected InetAddress address;
    protected DataChannel channel;
    protected ServerSocket serverSocket;
    // Listeners
    private ArrayList<ServerListener> serverInfoListeners;

    public Client(String host, int port) throws UnknownHostException {
        this.address = InetAddress.getByName(host);
        this.port = port;
        this.serverInfoListeners = new ArrayList<ServerListener>();
    }

    /*  Server info events */
    public void addServerInfoListener(ServerListener listener) {
        this.serverInfoListeners.add(listener);
    }

    public void removeServerInfoListener(ServerListener listener) {
        this.serverInfoListeners.remove(listener);
    }

    protected void fireServerInfo(String info) {
        for (int i = 0; i < serverInfoListeners.size(); i++) {
            serverInfoListeners.get(i).serverInfo(info);
        }
    }

    public void connect() throws IOException {

        Log.d(LOG_TAG, "Connectiong on port " + port + "...");

        this.serverSocket = new ServerSocket(Protocol.CLIENT_PORT);

        DataSocket clientChannel = new DataSocket(new Socket(address, port));

        Log.d(LOG_TAG, "Connected. Opening server channel...");

        DataSocket serverChannel = new DataSocket(serverSocket.accept());

        Log.d(LOG_TAG, "Server channel opened.");

        channel = new DataChannel(clientChannel, serverChannel);

        serverSocket.close();

        start();
    }
    /*
     * This function processes the server push messages
     */

    public void run() {
        while (isAlive()) {
            DataSocket serverChannel = channel.getServerChannel();
            try {
                int msg = serverChannel.readInt();
                if (!serverRequestHandler(msg)) {
                    throw new ProtocolException("Request not handled: " + msg);
                }
            } catch (IOException ex) {
                Log.e(LOG_TAG, ex);
                break;
            } catch (ProtocolException ex) {
                serverChannel.flushInputStream();
                Log.w(LOG_TAG, ex);
            }
        }
    }

    public String infoRequest(int infoID) throws IOException, ProtocolException, CommandException {
        DataSocket clientChannel = getDataChannel().getClientChannel();
        String result = null;

        clientChannel.writeInt(Protocol.M_INFO);
        clientChannel.writeInt(infoID);
        clientChannel.readResponse().assertOk();
        result = clientChannel.readUTF();
        clientChannel.writeAck();

        return result;
    }

    /*
     * This functions should handle requests from the server, and eventually
     * fire events to event listeners.
     * @return true if the request has been handled
     */
    protected boolean serverRequestHandler(int msg) throws IOException, ProtocolException {
        DataSocket serverChannel = getDataChannel().getServerChannel();
        switch (msg) {
            case Protocol.M_INFOMSG:
                String info = serverChannel.readUTF();
                fireServerInfo(info);
                serverChannel.writeAck();
                return true;
            case Protocol.M_CMD:
                int cmd = serverChannel.readInt();
                return serverCommandHandler(cmd);
        }
        return false;
    }
    /*
     * Handles commands from the server
     * @return true if command has been handled
     */

    protected abstract boolean serverCommandHandler(int cmd) throws IOException, ProtocolException;

    public DataChannel getDataChannel() {
        return channel;
    }

    public static interface ServerListener {

        public void serverInfo(String info);
    }
}
