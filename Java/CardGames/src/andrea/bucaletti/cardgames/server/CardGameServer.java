/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package andrea.bucaletti.cardgames.server;

import andrea.bucaletti.cardgames.cards.Card;
import andrea.bucaletti.net.game.protocol.DataChannel;
import andrea.bucaletti.net.game.protocol.ProtocolException;
import andrea.bucaletti.net.game.server.Server;
import java.io.IOException;

/**
 *
 * @author Andrea
 */
public abstract class CardGameServer extends Server {

    // Server Commands
    public static final int C_DEALCARD = 100;
    public static final int C_DEALCARD_COVERED = 101;
    public static final int C_GAMESTART = 102;
    // Client Commands
    public static final int C_JOINTEAM = 103;
    public static final int C_LEAVETEAM = 104;
    public static final int C_PLAYCARD = 105;
    public static final int C_GET_CARDSINDECK = 106;
    // Server info
    public static final int I_GAMENAME = 0;
    public static final int I_GAMESTATUS = 1;
    // Session info
    public static final String S_NAME = "NAME";
    public static final String S_TEAM = "TEAM";
    public static final String S_PLAYERINDEX = "INDEX";
    // Game status
    public static final String GAME_STATUS_LOBBY = "LOBBY";
    public static final String GAME_STATUS_PLAY = "RUNNING";
    protected int teams;
    protected int playersPerTeam;
    protected DataChannel[] orderedPlayers;

    public CardGameServer(int teams, int playersPerTeam, String gameName) {
        super();
        this.teams = teams;
        this.playersPerTeam = playersPerTeam;
        this.orderedPlayers = new DataChannel[teams * playersPerTeam];
        this.info.put(I_GAMENAME, gameName);
        this.info.put(I_GAMESTATUS, GAME_STATUS_LOBBY);
    }

    /*
     * @return The number of players in the specified team
     */
    public int getPlayersCount(int teamIndex) throws IllegalArgumentException {

        if (!validateTeamIndex(teamIndex)) {
            throw new IllegalArgumentException("Invalid team index");
        }

        int count = 0;
        for (int i = teamIndex; i < teams * playersPerTeam; i += playersPerTeam) {
            if (orderedPlayers[i] != null) {
                count++;
            }
        }

        return count;
    }

    /*
     * @return The next free player slot on the specified team, or -1 if the team is full
     */
    public int getNextFreePlayerSlot(int teamIndex) {
        int result = -1;

        if (!validateTeamIndex(teamIndex)) {
            throw new IllegalArgumentException("Invalid team index");
        }

        for (int i = teamIndex; i < teams * playersPerTeam; i += playersPerTeam) {
            if (orderedPlayers[i] == null) {
                result = i;
                break;
            }
        }

        return result;
    }

    public boolean isPlayer(DataChannel channel) {
        return channel.getValue(S_NAME) != null;
    }

    // client commands
    @Override
    protected boolean clientCommandHandler(DataChannel channel, int cmd) throws IOException, ProtocolException {

        switch (cmd) {
            case C_JOINTEAM:

                int teamIndex = channel.getClientChannel().readInt();
                String name = channel.getClientChannel().readUTF();



                synchronized (this) {

                    int slot = getNextFreePlayerSlot(teamIndex);

                    if (!validateTeamIndex(teamIndex) || slot == -1) {
                        channel.getClientChannel().writeErr("Can't join that team");
                    } else {
                        channel.putValue(S_NAME, name);
                        channel.putValue(S_TEAM, teamIndex);
                        channel.putValue(S_PLAYERINDEX, slot);
                        orderedPlayers[slot] = channel;
                        channel.getClientChannel().writeOk();
                    }
                }
                return true;
            case C_LEAVETEAM:

                synchronized (this) {
                    Integer playerIndex = channel.getValue(S_PLAYERINDEX);

                    if (playerIndex != null) {
                        orderedPlayers[playerIndex] = null;
                    }
                }

                channel.removeKey(S_NAME);
                channel.removeKey(S_TEAM);
                channel.removeKey(S_PLAYERINDEX);

                channel.getClientChannel().writeAck();

                return true;
            case C_PLAYCARD:

                Card card = channel.getClientChannel().readCard();

                if (!isPlayer(channel)) {
                    channel.getClientChannel().writeErr("Can't play that card");
                } else {
                    boolean result = playCardCommand(channel, card);
                    if (result) {
                        channel.getClientChannel().writeOk();
                    } else {
                        channel.getClientChannel().writeErr("Can't play that card");
                    }
                }

                return true;
            case C_GET_CARDSINDECK:

                channel.getClientChannel().writeInt(getCardsInDeck());
                channel.getClientChannel().readAck();

                return true;
        }
        return false;
    }

    protected abstract boolean playCardCommand(DataChannel player, Card card);

    public abstract int getCardsInDeck();

    // server commands
    public boolean startGame() throws IOException, ProtocolException {
        for (DataChannel p : orderedPlayers) {
            if (p == null) {
                return false;
            }
        }

        this.info.put(I_GAMESTATUS, GAME_STATUS_PLAY);

        for (DataChannel p : orderedPlayers) {
            p.getServerChannel().writeCommand(C_GAMESTART);
        }

        for (DataChannel p : orderedPlayers) {
            p.getServerChannel().readAck();
        }

        return true;
    }

    public void dealCard(Card card, DataChannel player) throws IOException, ProtocolException {

        int playerIndex = player.getValue(S_PLAYERINDEX);

        player.getServerChannel().writeCommand(C_DEALCARD);
        player.getServerChannel().writeCard(card);

        for (DataChannel p : orderedPlayers) {
            if (p != player) {
                p.getServerChannel().writeCommand(C_DEALCARD_COVERED);
                p.getServerChannel().writeInt(playerIndex);
            }
        }

        for (DataChannel p : orderedPlayers) {
            p.getServerChannel().readAck();
        }
    }

    private boolean validateTeamIndex(int teamIndex) {
        if (teamIndex < 0 || teamIndex >= teams) {
            return false;
        } else {
            return true;
        }
    }
}
