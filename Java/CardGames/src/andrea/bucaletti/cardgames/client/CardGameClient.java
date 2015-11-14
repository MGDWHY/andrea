/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.client;

import andrea.bucaletti.cardgames.cards.Card;
import andrea.bucaletti.cardgames.server.CardGameServer;
import andrea.bucaletti.net.game.client.Client;
import andrea.bucaletti.net.game.protocol.CommandException;
import andrea.bucaletti.net.game.protocol.DataSocket;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;

/**
 *
 * @author Andrea
 */
public abstract class CardGameClient extends Client {

    protected ArrayList<CardGameServerListener> cardGameServerListeners;

    public CardGameClient(String host, int port) throws UnknownHostException {
        super(host, port);
        cardGameServerListeners = new ArrayList<CardGameServerListener>();
    }

    public void addCardGameServerListener(CardGameServerListener listener) {
        cardGameServerListeners.add(listener);
    }

    public void removeCardGameServerListener(CardGameServerListener listener) {
        cardGameServerListeners.remove(listener);
    }

    // Client commands
    public void joinTeam(int teamIndex, String name) throws IOException, ProtocolException, CommandException {
        DataSocket clientChannel = getDataChannel().getClientChannel();
        clientChannel.writeCommand(CardGameServer.C_JOINTEAM);
        clientChannel.writeInt(teamIndex);
        clientChannel.writeUTF(name);
        clientChannel.readResponse().assertOk();
    }

    public void leaveTeam() throws IOException, ProtocolException {
        DataSocket clientChannel = getDataChannel().getClientChannel();
        clientChannel.writeCommand(CardGameServer.C_LEAVETEAM);
        clientChannel.readAck();
    }

    public void playCard(Card card) throws IOException, ProtocolException, CommandException {
        DataSocket clientChannel = getDataChannel().getClientChannel();
        clientChannel.writeCommand(CardGameServer.C_PLAYCARD);
        clientChannel.writeCard(card);
        clientChannel.readResponse().assertOk();
    }

    public int getCardsInDeck() throws IOException, ProtocolException {
        DataSocket clientChannel = getDataChannel().getClientChannel();
        clientChannel.writeCommand(CardGameServer.C_GET_CARDSINDECK);
        int cid = clientChannel.readInt();
        clientChannel.writeAck();
        return cid;
    }

    // Server Commands
    @Override
    protected boolean serverCommandHandler(int cmd) throws IOException, ProtocolException {
        DataSocket serverChannel = getDataChannel().getServerChannel();
        switch (cmd) {
            case CardGameServer.C_DEALCARD:
                Card card = serverChannel.readCard();
                fireDealCard(card);
                serverChannel.writeAck();
                return true;
            case CardGameServer.C_DEALCARD_COVERED:
                int playerIndex = serverChannel.readInt();
                fireDealCardCovered(playerIndex);
                serverChannel.writeAck();
                return true;
            case CardGameServer.C_GAMESTART:
                fireGameStart();
                serverChannel.writeAck();
                return true;
        }
        return false;
    }

    // Events
    protected void fireDealCard(Card card) {
        for (CardGameServerListener l : cardGameServerListeners) {
            l.dealCard(card);
        }
    }

    protected void fireDealCardCovered(int playerIndex) {
        for (CardGameServerListener l : cardGameServerListeners) {
            l.dealCardCovered(playerIndex);
        }
    }

    protected void fireGameStart() {
        for (CardGameServerListener l : cardGameServerListeners) {
            l.gameStart();
        }
    }

    public static interface CardGameServerListener {

        public void dealCard(Card card);

        public void dealCardCovered(int playerIndex);

        public void gameStart();
    }
}
