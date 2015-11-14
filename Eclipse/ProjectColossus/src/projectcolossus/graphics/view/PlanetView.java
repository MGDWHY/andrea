package projectcolossus.graphics.view;

import java.util.ArrayList;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.graphics.view.planetview.ConquestBar;
import projectcolossus.graphics.view.planetview.PowerBar;
import projectcolossus.graphics.view.planetview.UnitsView;
import projectcolossus.util.Util;
import andrea.bucaletti.projectcolossus.R;
import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class PlanetView extends RelativeLayout implements AdapterView.OnItemClickListener {
	
	protected static final int PLANET_UI_MARGINS = 10;
	
	protected static final int POWER_BAR_HEIGHT = 20;
	
	protected static final int CONQUEST_BAR_HEIGHT = 10;
	
	protected Activity owner;
	protected TextView txtPlanetName;
	protected PowerBar powerBar;
	protected ConquestBar conquestBar;
	protected LinearLayout layPlanetUI;
	
	protected Player currentPlayer;
	protected GameData gameData;
	protected Planet planet;
	protected int planetID;
	
	protected UnitsView unitsView;
	
	protected int selectedUnitID;
	
	public PlanetView(Context context, Activity owner, GameMap gameMap) {
		super(context);
		
		float density = context.getResources().getDisplayMetrics().density;
		int planetUIMargins = (int)Util.pixelToDP(PLANET_UI_MARGINS, density);
		
		View.inflate(context, R.layout.planet_info, this);
		
		this.setVisibility(View.INVISIBLE);
		this.owner = owner;
		this.planetID = 0;
		
		conquestBar = new ConquestBar(context);
		powerBar = new PowerBar(context);
		layPlanetUI = (LinearLayout)findViewById(R.id.layPlanetUI);
		unitsView = new UnitsView(context);
		txtPlanetName = (TextView)findViewById(R.id.txtPlanetName);	
		
		LinearLayout.LayoutParams params;
		
		// conquest bar
		layPlanetUI.addView(conquestBar);
		params = (LinearLayout.LayoutParams) conquestBar.getLayoutParams();
		params.width = LayoutParams.MATCH_PARENT;
		params.height = Util.pixelToDP(CONQUEST_BAR_HEIGHT, density);
		params.setMargins(planetUIMargins, planetUIMargins, planetUIMargins, 0);
		
		// power bar
		layPlanetUI.addView(powerBar);
		params = (LinearLayout.LayoutParams)powerBar.getLayoutParams();
		params.width = LayoutParams.MATCH_PARENT;
		params.height = Util.pixelToDP(POWER_BAR_HEIGHT, density);
		params.setMargins(planetUIMargins, 0, planetUIMargins, planetUIMargins);	
		
		// units view
		layPlanetUI.addView(unitsView);
		unitsView.setOnItemClickListener(this);
		params = (LinearLayout.LayoutParams) unitsView.getLayoutParams();
		params.width = LayoutParams.MATCH_PARENT;
		params.height = LayoutParams.WRAP_CONTENT;
		params.setMargins(planetUIMargins, planetUIMargins, planetUIMargins, planetUIMargins);	
	}
	
	public int getSelectedUnitID() { return selectedUnitID; }
	public void setSelectedUnitID(int id) { this.selectedUnitID = id; }
	
	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		Unit u = (Unit)unitsView.getAdapter().getItem(position);
		
		if(currentPlayer.getUnitByID(u.getID()) != null) {
			selectedUnitID = u.getID();
			unitsView.setSelectedIndex(position);
		}
	}
	
	public void update(Player currentPlayer, GameData gameData) {
		this.currentPlayer = currentPlayer;
		this.gameData = gameData;
		
		showPlanetInfo(this.planetID);
	}
	
	public void showPlanetInfo(int planetID) {
		this.planetID = planetID;
		this.planet = gameData.getPlanetByID(planetID);
		
		if(planet != null) {	
			conquestBar.update(gameData, planetID, currentPlayer.getIndex());
			powerBar.update(currentPlayer, gameData, planet);
			unitsView.setUnits(currentPlayer, planet.getUnits());				
		}
		
		owner.runOnUiThread( new ShowPlanetInfoRunnable(planet));
	}
	
	private class ShowPlanetInfoRunnable implements Runnable {
		
		private Planet planet;
		
		public ShowPlanetInfoRunnable(Planet planet) {
			this.planet = planet;
		}
		
		@Override
		public void run() {
			
			if(currentPlayer == null || gameData == null)
				return;
			
			ArrayList<Planet> visiblePlanets = gameData.getVisiblePlanets(currentPlayer);
			
			if(planet == null) {
				setVisibility(View.GONE);
			} else {				
				
				txtPlanetName.setText(planet.getName());
				
				if(!visiblePlanets.contains(planet)) {
					
					txtPlanetName.setTextColor(0xffcccccc);
					conquestBar.setVisibility(View.GONE);
					powerBar.setVisibility(View.GONE);
					unitsView.setVisibility(View.GONE);
					
				} else {
					
					if(planet.getUnits().size() > 0) {
						((BaseAdapter)unitsView.getAdapter()).notifyDataSetChanged();
						powerBar.postInvalidate();
						powerBar.setVisibility(View.VISIBLE);
						unitsView.setVisibility(View.VISIBLE);
					} else {
						powerBar.setVisibility(View.GONE);
						unitsView.setVisibility(View.GONE);
					}
					
					if(!currentPlayer.equals(planet.getOwner())) {
						conquestBar.postInvalidate();
						conquestBar.setVisibility(View.VISIBLE);
					} else {
						conquestBar.setVisibility(View.GONE);
					}
					
					if(planet.isOwned()) {
						txtPlanetName.setTextColor(planet.getOwner().getColor());		
					} else {			
						txtPlanetName.setTextColor(0xffCCCCCC);	
					}
				}
				
				setVisibility(View.VISIBLE);
			}
		}
		
	}



}
