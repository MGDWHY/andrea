package projectcolossus.graphics;

import java.net.InetAddress;

import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.heirs.AegisCard;
import projectcolossus.gamelogic.cards.heirs.HunterCard;
import projectcolossus.gamelogic.cards.heirs.RaiderCard;
import projectcolossus.gamelogic.cards.heirs.ScoutCard;
import projectcolossus.gamelogic.client.Client;
import projectcolossus.gamelogic.server.ServerCommand;
import projectcolossus.graphics.dialog.CardDialogFragment;
import projectcolossus.graphics.view.CardBigView;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.projectcolossus.R;
import andrea.bucaletti.projectcolossus.R.layout;
import andrea.bucaletti.projectcolossus.R.menu;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class GameStartActivity extends Activity implements View.OnClickListener {

	private Button cmdStartLocalGame, cmdStartNetworkGame, cmdPreviewCard;
	private EditText txtMMServerIP;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		ResourceLoader.initialize(getApplicationContext());	
		
		setContentView(R.layout.activity_game_start);
		
		cmdStartLocalGame = (Button)findViewById(R.id.cmdStartLocalGame);
		cmdStartNetworkGame = (Button)findViewById(R.id.cmdStartNetworkGame);
		cmdPreviewCard = (Button)findViewById(R.id.cmdPreviewCard);
		txtMMServerIP = (EditText)findViewById(R.id.txtServerIP);
		
		cmdStartLocalGame.setOnClickListener(this);
		cmdStartNetworkGame.setOnClickListener(this);
		cmdPreviewCard.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		if(v == cmdStartLocalGame) {
			Intent intent = new Intent(this, LocalGameActivity.class);
			startActivity(intent);
		} else if(v == cmdStartNetworkGame) {
			new StartNetworkGameThread(this, txtMMServerIP.getText().toString()).start();
		} else if(v == cmdPreviewCard) {
			CardDialogFragment frg = new CardDialogFragment();
			frg.setCard(Card.forName(RaiderCard.NAME));
			frg.show(getFragmentManager(), "CardDialogFragment");
		}
	}
	
	private class StartNetworkGameThread extends Thread {
		
		private Activity owner;
		private String serverIP;	
		
		public StartNetworkGameThread(Activity owner, String serverIP) {
			this.owner = owner;
			this.serverIP = serverIP;
		}
		
		public void run() {
			try {
				Client client = new Client();
				ServerCommand.MMGameServerData gameServerData = client.findMatch(InetAddress.getByName(serverIP), 10000);
				gameServerData.ServerAddress = serverIP;
				Intent intent = new Intent(owner, NetworkGameActivity.class);
				intent.putExtra(NetworkGameActivity.EXTRA_GAMESERVER, gameServerData);
				owner.startActivity(intent);
			}
			catch(Exception ex) {
				ex.printStackTrace();	
			}
		}
		
	}

}
