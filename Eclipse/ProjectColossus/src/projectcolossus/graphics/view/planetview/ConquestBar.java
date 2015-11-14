package projectcolossus.graphics.view.planetview;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.view.View;

public class ConquestBar extends View {
	
	private static final int BAR_BORDER_COLOR = 0xff000000;
	private static final int BAR_BACKCOLOR = 0xff666666;
	
	private RectF target;
	private RectF dest;
	
	private Paint fgPaint, bgPaint, brPaint;
	
	private float conquestRatio;

	public ConquestBar(Context context) {
		super(context);
		target = new RectF();
		dest = new RectF();
		fgPaint = new Paint();
		brPaint = new Paint();
		bgPaint = new Paint();
		
		bgPaint.setColor(BAR_BACKCOLOR);
		
		brPaint.setColor(BAR_BORDER_COLOR);
		brPaint.setStyle(Paint.Style.STROKE);
	}
	
	public void update(GameData gameData, int planetID, int playerIndex) {
		Planet planet = gameData.getPlanetByID(planetID);
		Player player = gameData.getPlayer(playerIndex);
		
		if(planet != null && player != null) {
			GameData.ConquestData data = planet.getConquestData();
			fgPaint.setColor(player.getColor());
			conquestRatio = (float)data.getConquestCounter(playerIndex) / data.getMaxValue();
		}
	}
	
	@Override
	public void onDraw(Canvas canvas) {
		target.set(0, 0, getWidth(), getHeight());
		dest.set(0, 0, getWidth() * conquestRatio, getHeight());
		
		canvas.drawRect(target, bgPaint); // background
		
		canvas.drawRect(dest, fgPaint);	// conquested
		
		canvas.drawRect(target, brPaint); // border;
	}
	
}
