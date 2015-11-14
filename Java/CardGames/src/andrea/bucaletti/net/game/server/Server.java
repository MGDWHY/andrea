/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.net.game.server;

/**
 *
 * @author Andrea
 */
import andrea.bucaletti.cardgames.util.Log;
import andrea.bucaletti.net.game.protocol.DataChannel;
import andrea.bucaletti.net.game.protocol.DataSocket;
import andrea.bucaletti.net.game.protocol.Protocol;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;

// Generic game server
public abstract class Server extends Thread {

    // Server commands
    private static final String LOG_TAG = "Server";
    protected ServerSocket serverSocket;
    protected ArrayList<DataChannel> clients;
    protected HashMap<Integer, String> info;
    protected int port;
    protected boolean running = false;

    public Server() {
        this(Protocol.SERVER_PORT);
    }

    public Server(int port) {
        this.port = port;
        this.clients = new ArrayList<DataChannel>();
        this.info = new HashMap<Integer, String>();

        try {
            this.serverSocket = new ServerSocket(port);
        } catch (IOException ex) {
            Log.e(LOG_TAG, ex);
        }
    }

    public void putInfo(int id, String descr) {
        info.put(id, descr);
    }

    public void run() {
        running = true;

        Log.d(LOG_TAG, "Server listening on port " + port);

        while (running) {
            try {
                DataSocket clientChannel = new DataSocket(serverSocket.accept());
                Log.d(LOG_TAG, "Connection accepted. Opening server channel...");
                DataSocket serverChannel = new DataSocket(new Socket(clientChannel.socket.getInetAddress(), Protocol.CLIENT_PORT));
                Log.d(LOG_TAG, "Server channel opened");
                DataChannel channel = new DataChannel(clientChannel, serverChannel);

                synchronized (clients) {
                    clients.add(channel);
                    new ClientThread(channel).start();
                }

            } catch (IOException ex) {
                Log.e(LOG_TAG, ex);
            }
            Thread.yield();
        }

        Log.d(LOG_TAG, "Closing");

        running = false;
    }

    /*
     * Client push handler thread
     */
    private class ClientThread extends Thread {

        private DataChannel channel;

        public ClientThread(DataChannel channel) {
            this.channel = channel;
        }

        public void run() {

            while (running) {
                DataSocket clientChannel = channel.getClientChannel();
                try {
                    int msg = clientChannel.readInt();
                    Log.d(LOG_TAG, "Client request: " + msg);
                    if (!clientRequestHandler(channel, msg)) {
                        throw new ProtocolException("Request not handled: " + msg);
                    }
                } catch (ProtocolException ex) {
                    clientChannel.flushInputStream();
                    Log.w(LOG_TAG, ex);
                } catch (IOException ex) {
                    Log.e(LOG_TAG, ex);
                    break;
                }
            }


            try {
                disconnectClient(channel);
            } catch (IOException ex) {
            }
        }
    }

    public void disconnectClient(DataChannel client) throws IOException {
        synchronized (clients) {
            onClientDisconnected(client);
            client.close();
            clients.remove(client);
        }
    }

    /*
     * This function processes clients' requests.
     * @ return true if the request has been handled
     */
    protected boolean clientRequestHandler(DataChannel channel, int msg) throws IOException, ProtocolException {
        DataSocket clientChannel = channel.getClientChannel();
        switch (msg) {
            case Protocol.M_INFO:
                int i = clientChannel.readInt();
                if (info.containsKey(i)) {
                    clientChannel.writeOk();
                    clientChannel.out.writeUTF(info.get(i));
                    clientChannel.readAck();
                } else {
                    clientChannel.writeErr("Info not found");
                    clientChannel.readAck();
                }
                return true;
            case Protocol.M_CMD:
                int cmd = clientChannel.readInt();
                return clientCommandHandler(channel, cmd);
            default:
                return false;
        }


    }
    /*
     * Processes clients' commands
     * @ return true if command has been handled
     */

    protected abstract boolean clientCommandHandler(DataChannel channel, int cmd) throws IOException, ProtocolException;

    protected abstract void onClientDisconnected(DataChannel client);

    public void infoBroadcast(String info) throws IOException, ProtocolException {
        synchronized (clients) {
            for (int i = 0; i < clients.size(); i++) {
                DataSocket serverChannel = clients.get(i).getServerChannel();
                serverChannel.writeInt(Protocol.M_INFOMSG);
                serverChannel.out.writeUTF(info);
            }

            for (int i = 0; i < clients.size(); i++) {
                DataSocket serverChannel = clients.get(i).getServerChannel();
                serverChannel.readAck();
            }
        }
    }
}
