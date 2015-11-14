package projectcolossus.graphics.view.planetview;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.util.Util;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.View;

public class PowerBar extends View {
	
	private static final int PB_TEXT_HEIGHT = 16;
	
	protected Planet planet;
	protected GameData gameData;
	protected Player currentPlayer;
	
	protected Paint ptBackground;
	protected Paint ptText;
	protected Paint ptBorder;
	protected Rect rTextBounds;
	
	protected Player[] players;
	protected float totalPower;
	protected int powers[];
	protected float ratios[];
	
	public PowerBar(Context context) {
		super(context);	
		
		rTextBounds = new Rect();
		
		ptBackground = new Paint();
		ptText = new Paint();
		ptBorder = new Paint();
		
		ptText.setTextSize(Util.pixelToDP(PB_TEXT_HEIGHT, context.getResources().getDisplayMetrics().density));
		ptText.setColor(Color.WHITE);
		
		
	}
	
	public void update(Player currentPlayer, GameData gameData, Planet planet) {
		this.currentPlayer = currentPlayer;
		this.gameData = gameData;
		this.planet = planet;
		
		if(this.currentPlayer == null || this.gameData == null || planet == null)
			return;
		
		players = gameData.getPlayers();
		totalPower = 0.0f;
		
		powers = new int[players.length];
		ratios = new float[players.length];
		
		for(Unit u : planet.getUnits()) {
			powers[u.getPlayer().getIndex()] += u.getPower();
			totalPower += u.getPower();	
		}
		
		if(totalPower == 0.0f) {
			for(int i = 0; i < players.length; i++)
				ratios[i] = 1.0f / players.length;
		} else {
			for(int i = 0; i < players.length; i++)
				ratios[i] = powers[i] / totalPower;
		}	
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		if(this.currentPlayer == null || this.gameData == null || planet == null)
			return;
		
		float currentX = 0;
		
		ptBackground.setStyle(Paint.Style.FILL);
		
		for(int i = 0; i < players.length; i++) {
			String text = powers[i] + "";
			float w = ratios[i] * getWidth();
			
			ptText.getTextBounds(text, 0, text.length(), rTextBounds);
			ptBackground.setColor(players[i].getColor());
			
			canvas.drawRect(currentX, 0, currentX + w, getHeight(), ptBackground);
			
			if(powers[i] > 5) {
				canvas.drawText(text, 
						currentX + w / 2 - rTextBounds.width() / 2, 
						getHeight() / 2 + rTextBounds.height() / 2,
						ptText);
			}	
			
			currentX += w;
		}
		
		ptBackground.setStyle(Paint.Style.STROKE);
		ptBackground.setColor(0xff000000);
		canvas.drawRect(0, 0, getWidth(), getHeight(), ptBackground);
	}

}
