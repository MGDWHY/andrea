/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package projectconquest.net;

import java.io.IOException;
import java.util.HashMap;

/**
 *
 * @author Andrea
 *
 * Class to manage a bidirectional connection with session control. Clients do
 * their requests (ie start the communication) on the client channel, while the
 * server uses serverChannel to push events to the clients
 *
 */
public class DataChannel {

    private DataSocket clientChannel;
    private DataSocket serverChannel;
    private HashMap<String, Object> session;

    public DataChannel(DataSocket clientChannel, DataSocket serverChannel) {
        this.clientChannel = clientChannel;
        this.serverChannel = serverChannel;
        this.session = new HashMap<String, Object>();
    }

    public void putValue(String key, Object value) {
        session.put(key, value);
    }

    public <T> T getValue(String key) {
        if (session.containsKey(key)) {
            return (T) session.get(key);
        } else {
            return null;
        }
    }

    public void removeKey(String key) {
        session.remove(key);
    }

    public DataSocket getClientChannel() {
        return clientChannel;
    }

    public DataSocket getServerChannel() {
        return serverChannel;
    }

    public void close() throws IOException {
        clientChannel.close();
        serverChannel.close();
    }
}
