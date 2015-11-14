package projectcolossus.graphics.view;

import java.util.ArrayList;

import andrea.bucaletti.projectcolossus.R;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.client.Client;

public class PlayerView extends RelativeLayout implements View.OnClickListener {
	
	private Activity owner;
	
	private Player player, currentPlayer;
	
	private RelativeLayout layPlayer;
	private LinearLayout layCards;
	private HorizontalScrollView hsvCards;	
	
	private ImageView cmdShowHand;
	private ImageView cmdEndTurn;
	
	private TextView txtNumCards;
	private TextView txtResources;
	
	private ArrayList<Listener>listeners;
	
	public PlayerView(Context context, Activity owner) {
		super(context);
		
		this.owner = owner;
		this.listeners = new ArrayList<Listener>();
		
		View.inflate(context, R.layout.player_hand, this);
		
		layPlayer = (RelativeLayout)findViewById(R.id.layPlayer);
		layCards = (LinearLayout)findViewById(R.id.layCards);
		cmdShowHand = (ImageView)findViewById(R.id.cmdShowHand);
		cmdEndTurn = (ImageView)findViewById(R.id.cmdEndTurn);
		hsvCards = (HorizontalScrollView) findViewById(R.id.hsvCards);
		txtNumCards = (TextView) findViewById(R.id.txtNumCards);
		txtResources = (TextView) findViewById(R.id.txtResources);
		
		cmdEndTurn.setOnClickListener(this);
		cmdShowHand.setOnClickListener(this);
		hsvCards.setVisibility(View.GONE);
	}
	
	public boolean addListener(Listener l) { return listeners.add(l); }
	public boolean removeListener(Listener l) { return listeners.remove(l); }
	/*
	 * The current player is the player currenlty playing his turn
	 * This is needed to highlight the top bar with the player's color
	 */
	public void setCurrentPlayer(Player currentPlayer) {
		this.currentPlayer = currentPlayer;
		update();
	}
	
	/*
	 * The player is the one of which we're showing his hand
	 * In local game currentPlayer = player, while in a network game
	 * it's not
	 */
	public void setPlayer(Player player) {
		this.player = player;
		update();
	}
	
	public void update() {
		if(this.player != null)
			owner.runOnUiThread(new UpdateRunnable(this.player, this.currentPlayer));
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v == cmdShowHand) {
			owner.runOnUiThread(new TogglePlayerHandRunnable());
		} else if(v == cmdEndTurn) {
			fireOnEndTurn();
		}
	}
	
	protected void fireOnEndTurn() {
		for(Listener l : listeners)
			l.onEndTurnClick(player);
	}
	
	public static interface Listener {
		public void onEndTurnClick(Player player);
	}
	
	private class UpdateRunnable implements Runnable {
		
		private Player player, currentPlayer;
		
		public UpdateRunnable(Player player, Player currentPlayer) {
			this.player = player;
			this.currentPlayer = currentPlayer;
		}
		
		@Override
		public void run() {
			
			if(currentPlayer != null) {
				layPlayer.setBackgroundColor(currentPlayer.getColor());
			}
			
			if(player != null) {
				
				if(player.equals(currentPlayer))
					txtResources.setVisibility(View.VISIBLE);
				else
					txtResources.setVisibility(View.GONE);
				
				ArrayList<Card> hand = player.getHand();
				
				layCards.removeAllViews();
				
				txtNumCards.setText(hand.size() + "");
				
				txtResources.setText("Resources: " + player.getResources());
				
				LayoutParams params = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
				
				for(int i = 0; i < hand.size(); i++) {
					CardLittleView view = new CardLittleView(owner, i, hand.get(i));
					layCards.addView(view, params);				
				}
			}
		}
	}	
	private class TogglePlayerHandRunnable implements Runnable {
		public void run() {
			if(hsvCards.getVisibility() == View.VISIBLE)
				hsvCards.setVisibility(View.GONE);
			else {
				hsvCards.setVisibility(View.VISIBLE);
			}			
		}
	}
}
