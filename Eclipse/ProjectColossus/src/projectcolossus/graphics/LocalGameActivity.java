package projectcolossus.graphics;

import java.io.IOException;
import java.net.InetAddress;
import java.util.Currency;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.cards.FreeCard;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.player.HumanPlayer;
import projectcolossus.gamelogic.server.Server;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.gamelogic.client.Client;
import projectcolossus.graphics.animation.Animation;
import projectcolossus.graphics.animation.ExplosionAnimation;
import projectcolossus.graphics.animation.MoveUnitAnimation;
import projectcolossus.graphics.dialog.YesNoDialogFragment;
import projectcolossus.graphics.view.GameView;
import projectcolossus.graphics.view.PlanetView;
import projectcolossus.graphics.view.PlayerView;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.projectcolossus.R;
import android.media.AudioManager;
import android.os.Bundle;
import android.app.Activity;
import android.app.DialogFragment;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;

public class LocalGameActivity extends Activity implements Client.Listener, GameView.Listener, 
	PlayerView.Listener, YesNoDialogFragment.Listener{
	
	
	private Client currentClient;
	private Server server;
	
	private GameData gameData;
	private GameMap gameMap;
	private GameView gameView;
	private PlanetView planetInfoView;
	private PlayerView playerHandView;
	
	private FrameLayout layout;
	
	private YesNoDialogFragment frgEndTurn;
	
	private int serverPort;
	
	private Client[] clients = new Client[2];
	private Deck[] decks = new Deck[2];
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		
		ResourceLoader.initialize(getApplicationContext());
		ResourceLoader.loadCardTinyImages();
		
		try {
			this.gameMap = GameMap.load(getResources().openRawResource(R.raw.map));
		}
		catch(Exception ex) {
			ex.printStackTrace();
			System.exit(-1);
		}
		
		try {
			server = new Server(10000, gameMap);
			serverPort = server.getListeningPort();
			
			for(int i = 0; i < clients.length; i++) {
				clients[i] = new Client();
				clients[i].addListener(this);
				decks[i] = Deck.read(getResources().openRawResource(R.raw.deck));
			}
		}
		catch(Exception ex) {
			Log.e("Test3", ex.getMessage());
		}
		layout = new FrameLayout(getApplicationContext());
		
		planetInfoView = new PlanetView(getApplicationContext(), this, gameMap);
		gameView = new GameView(getApplicationContext(), this, planetInfoView, gameMap);
		playerHandView = new PlayerView(getApplicationContext(), this);
		
		playerHandView.addListener(this);
		gameView.addListener(this);
		
		layout.addView(gameView);
		layout.addView(planetInfoView);
		layout.addView(playerHandView);
		
		frgEndTurn = new YesNoDialogFragment();
		frgEndTurn.setMessage("Do you really want to end your turn?");
		frgEndTurn.addListener(this);			
		
		setContentView(layout);
	}
	
	@Override
	protected void onStart() {
		super.onStart();
		new Thread() {
			public void run() {
				try {
					gameView.waitLoaded();
					for(int i = 0; i < clients.length; i++) {
						clients[i].connect(InetAddress.getLocalHost(), serverPort, "Player " + i, decks[i]);
					}
				} catch(Exception ex) {
					Log.e("Test3", ex.getMessage());
				}
			}
		}.start();			
	}

	@Override
	public void onUpdate(Client client, Player player, GameData gameData) {
		this.gameData = gameData;
		gameView.update(gameData.getCurrentPlayer(), gameData);
		planetInfoView.update(gameData.getCurrentPlayer(), gameData);
		playerHandView.setPlayer(gameData.getCurrentPlayer());
		playerHandView.setCurrentPlayer(gameData.getCurrentPlayer());		
	}

	@Override
	public void onBeginTurn(Client client, int currentPlayer) {
		currentClient = client;
	}
	
	@Override
	public void onBadRequest(Client client, int refCommand, int errorCode) {

	}	
	
	@Override
	public void onUnitMoved(final Client client, int unitID, int fromPlanetID, int toPlanetID) {
		
		if(client == currentClient) {
			
			Planet from = gameData.getPlanetByID(fromPlanetID);
			Planet to = gameData.getPlanetByID(toPlanetID);
			Unit unit = gameData.getUnitByID(unitID);
			
			Animation anim = new MoveUnitAnimation(unit.getPlayer(), from, to);
			/*
			Animation.EndCallback callback = new Animation.EndCallback() {
				@Override
				public void onAnimationEnd(Animation animation) {
					client.requestUpdate(true);
				}
			};
			
			anim.setEndCallback(callback);*/
			
			gameView.getAnimationExecutor().runAnimation(anim);	
			
		}
	}	
	
	@Override
	public void onPlanetCardDropped(PlanetCard card, int cardIndex, int planetID) {
		Planet planet = gameData.getPlanetByID(planetID);
		if(card.isPlayable() && card.canPlayOnPlanet(gameData.getGameMap(), planet)) {
			currentClient.playPlanetCard(cardIndex, planetID, true);
		}
	}	

	@Override
	public void onFreeCardDropped(FreeCard card, int cardIndex) {
		if(card.isPlayable() && card.canPlayRegardless()) {
			currentClient.playFreeCard(cardIndex, true);
		}
	}

	@Override
	public void onEndTurnClick(Player player) {
		// TODO Auto-generated method stub
	
		frgEndTurn.show(getFragmentManager(), "YesNoFragmentDialog");
	}

	@Override
	public void onPlanetClicked(int planetID) {
		int uid = planetInfoView.getSelectedUnitID();
		
		if(planetInfoView.getSelectedUnitID() > 0 && planetID > 0) {
			Unit u = gameData.getCurrentPlayer().getUnitByID(uid);
			
			currentClient.moveUnit(u.getID(), u.getPlanet().getID(), planetID, true);
			
			planetInfoView.setSelectedUnitID(0);

		} else {
			planetInfoView.showPlanetInfo(planetID);
			gameView.gotoPlanet(planetID);
		}
	}

	@Override
	public void onDialogResult(DialogFragment dialogFragment, int value) {
		if(value == YesNoDialogFragment.DIALOG_YES) {
			currentClient.endTurn(true);
			planetInfoView.setSelectedUnitID(0);	
		}
	}









}
