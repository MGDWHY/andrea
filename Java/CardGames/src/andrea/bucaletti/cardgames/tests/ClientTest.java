/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.tests;

import andrea.bucaletti.cardgames.cards.Card;
import andrea.bucaletti.cardgames.client.CardGameClient;
import andrea.bucaletti.cardgames.server.CardGameServer;
import andrea.bucaletti.net.game.protocol.CommandException;
import andrea.bucaletti.net.game.protocol.Protocol;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 *
 * @author Andrea
 */
public class ClientTest extends CardGameClient implements CardGameClient.CardGameServerListener {

    public ClientTest() throws UnknownHostException {
        super("localhost", Protocol.SERVER_PORT);
        addCardGameServerListener(this);
    }

    @Override
    public void dealCard(Card card) {
        println("Card received: " + card.getStringRepresentation());
    }

    @Override
    public void dealCardCovered(int playerIndex) {
        println("Player " + playerIndex + " received a card");
    }

    @Override
    public void gameStart() {
        println("Game has started");
    }

    public static void main(String[] args) throws IOException, ProtocolException {
        ClientTest client = new ClientTest();
        HashMap<String, Integer> commands = new HashMap<String, Integer>();

        commands.put("playcard", CardGameServer.C_PLAYCARD);
        commands.put("jointeam", CardGameServer.C_JOINTEAM);
        commands.put("leaveteam", CardGameServer.C_LEAVETEAM);
        commands.put("cardscount", CardGameServer.C_GET_CARDSINDECK);

        client.connect();

        while (true) {

            String line = prompt("");
            StringTokenizer st = new StringTokenizer(line, " ");

            String cmdStr = st.nextToken();

            Integer cmd = commands.get(cmdStr);



            if (cmd != null) {
                try {
                    switch (cmd) {
                        case CardGameServer.C_PLAYCARD:

                            int rank = Integer.parseInt(st.nextToken());
                            int suit = Integer.parseInt(st.nextToken());
                            Card card = new Card(suit, rank);

                            client.playCard(card);

                            break;

                        case CardGameServer.C_JOINTEAM:

                            String name = st.nextToken();
                            int team = Integer.parseInt(st.nextToken());

                            client.joinTeam(team, name);

                            break;

                        case CardGameServer.C_LEAVETEAM:

                            client.leaveTeam();

                            break;

                        case CardGameServer.C_GET_CARDSINDECK:

                            int cardsInDeck = client.getCardsInDeck();

                            println("There are " + cardsInDeck + " cards left");

                            break;

                    }
                } catch (CommandException ex) {
                    println("Command Exception: " + ex.getMessage());
                }

            }

        }

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
