package projectcolossus.graphics;

import java.io.IOException;
import java.net.InetAddress;
import java.util.ArrayList;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Deck;
import projectcolossus.gamelogic.cards.FreeCard;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.gamelogic.client.Client;
import projectcolossus.gamelogic.server.Server;
import projectcolossus.gamelogic.server.ServerCommand;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.graphics.animation.Animation;
import projectcolossus.graphics.animation.MoveUnitAnimation;
import projectcolossus.graphics.dialog.YesNoDialogFragment;
import projectcolossus.graphics.view.GameView;
import projectcolossus.graphics.view.PlanetView;
import projectcolossus.graphics.view.PlayerView;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.projectcolossus.R;
import android.app.Activity;
import android.app.DialogFragment;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.FrameLayout;

public class NetworkGameActivity extends Activity implements Client.Listener, GameView.Listener, 
PlayerView.Listener, YesNoDialogFragment.Listener{
	
	public static final String EXTRA_GAMESERVER = "1e7da2ba0d98dea3c58af1e2bf7d6bfb";
	
	private int playerIndex;
	
	private ServerCommand.MMGameServerData gameServerData;

	private Client client;
	private Deck deck;
	
	private GameData gameData;
	private GameMap gameMap;
	
	private GameView gameView;
	private PlanetView planetInfoView;
	private PlayerView playerHandView;
	
	private FrameLayout layout;
	
	private YesNoDialogFragment frgEndTurn;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = getIntent();
		
		ResourceLoader.initialize(getApplicationContext());
		ResourceLoader.loadCardTinyImages();
		
		gameServerData = (ServerCommand.MMGameServerData) intent.getExtras().get(EXTRA_GAMESERVER);
		
		try {
			this.deck = Deck.read(getResources().openRawResource(R.raw.deck));
			this.gameMap = gameServerData.GameMap;
			this.client = new Client();
			this.client.addListener(this);
		}
		catch(IOException ex) {
			ex.printStackTrace();
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
					playerIndex = client.connect(InetAddress.getByName(gameServerData.ServerAddress), 
							gameServerData.ServerPort, "Playor", deck);
				} catch(Exception ex) {
					Log.e("Test3", ex.getMessage());
				}
			}
		}.start();			
	}

	@Override
	public void onUpdate(Client client, Player player, GameData gameData) {
		this.gameData = gameData;
		gameView.update(gameData.getPlayer(playerIndex), gameData);
		planetInfoView.update(gameData.getPlayer(playerIndex), gameData);
		playerHandView.setPlayer(gameData.getPlayer(playerIndex));
		playerHandView.setCurrentPlayer(gameData.getCurrentPlayer());
	}

	@Override
	public void onBeginTurn(Client client, int currentPlayer) {

	}
	
	@Override
	public void onBadRequest(Client client, int refCommand, int errorCode) {

	}	
	
	@Override
	public void onUnitMoved(Client client, int unitID, int fromPlanetID, int toPlanetID) {
		
		ArrayList<Planet> visiblePlanets = gameData.getVisiblePlanets(playerIndex);
		
		Planet from = gameData.getPlanetByID(fromPlanetID);
		Planet to = gameData.getPlanetByID(toPlanetID);
		Unit unit = gameData.getUnitByID(unitID);
		
		if(visiblePlanets.contains(from) || visiblePlanets.contains(to)) {
			Animation anim = new MoveUnitAnimation(unit.getPlayer(), from, to);
			gameView.getAnimationExecutor().runAnimation(anim);			
		}
	}	
	
	@Override
	public void onPlanetCardDropped(PlanetCard card, int cardIndex, int planetID) {
		Planet planet = gameData.getPlanetByID(planetID);
		if(card.isPlayable() && card.canPlayOnPlanet(gameData.getGameMap(), planet)) {
			client.playPlanetCard(cardIndex, planetID, true);
		}
	}	

	@Override
	public void onFreeCardDropped(FreeCard card, int cardIndex) {
		if(card.isPlayable() && card.canPlayRegardless()) {
			client.playFreeCard(cardIndex, true);
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
			Unit u = gameData.getPlayer(playerIndex).getUnitByID(uid);
			
			client.moveUnit(u.getID(), u.getPlanet().getID(), planetID, true);
			
			planetInfoView.setSelectedUnitID(0);

		} else {
			planetInfoView.showPlanetInfo(planetID);
			gameView.gotoPlanet(planetID);
		}
		
	}

	@Override
	public void onDialogResult(DialogFragment dialogFragment, int value) {
		if(value == YesNoDialogFragment.DIALOG_YES) {
			client.endTurn(true);
			planetInfoView.setSelectedUnitID(0);	
		}
	}











}
