/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.tests;

/**
 *
 * @author Andrea
 */
import andrea.bucaletti.cardgames.cards.Card;
import andrea.bucaletti.cardgames.cards.CardsDeck;
import andrea.bucaletti.cardgames.server.CardGameServer;
import andrea.bucaletti.net.game.protocol.DataChannel;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.StringTokenizer;

public class ServerTest extends CardGameServer {

    CardsDeck deck;

    public ServerTest() {
        super(2, 1, "TestServer");
        deck = CardsDeck.createCardsDeck40();
    }

    public static void main(String[] args) throws IOException, ProtocolException {
        ServerTest server = new ServerTest();
        HashMap<String, Integer> commands = new HashMap<String, Integer>();

        commands.put("gamestart", C_GAMESTART);
        commands.put("deal", C_DEALCARD);

        server.start();

        while (true) {
            String line = prompt("");
            StringTokenizer st = new StringTokenizer(line, " ");

            String cmdStr = st.nextToken();

            Integer cmd = commands.get(cmdStr);

            if (cmd != null) {

                switch (cmd) {
                    case C_DEALCARD:

                        int playerIndex = Integer.parseInt(st.nextToken());
                        int rank = Integer.parseInt(st.nextToken());
                        int suit = Integer.parseInt(st.nextToken());
                        Card card = new Card(suit, rank);

                        server.dealCard(card, server.orderedPlayers[playerIndex]);

                        break;

                    case C_GAMESTART:

                        server.startGame();

                        break;

                }

            }

        }

    }

    @Override
    protected void onClientDisconnected(DataChannel client) {
        return;
    }

    @Override
    protected boolean playCardCommand(DataChannel player, Card card) {
        println(player.getValue(S_NAME) + " played " + card.getStringRepresentation());
        return true;
    }

    @Override
    public int getCardsInDeck() {
        return deck.getCardsCount();
    }

    // Utility
    public static void print(String str) {
        System.out.print(str);
    }

    public static void println(String str) {
        System.out.println(str);
    }

    public static String prompt(String prompt) throws IOException {
        print(prompt);
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        return reader.readLine();
    }
}