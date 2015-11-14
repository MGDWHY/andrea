package projectcolossus.graphics.animation;

import java.util.ArrayList;
import java.util.List;

import andrea.bucaletti.android.lib.Timer;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.util.PRNG;

public class ExplosionGenerator {
	
	private static final float MAX_UNITS_COUNT = 3;
	//private static final int MAX_EXPLOSIONS_PER_SECOND = 20;
	
	protected Timer timer;
	
	protected List<Planet> visiblePlanets;
	
	protected AnimationExecutor executor;
		
	public ExplosionGenerator(AnimationExecutor executor) {
		this.executor = executor;
		this.visiblePlanets = new ArrayList<Planet>();
		this.timer = new Timer();
	}
	
	public void setVisiblePlanets(List<Planet> visiblePlanets) {
		this.visiblePlanets = visiblePlanets;
	}
	
	public void generateExplosions() {
		
		float dt = timer.dt();
		
		for(Planet planet : visiblePlanets) {
			
			if(!planet.isContested())
				continue;
			
			float probability = dt * Math.min(planet.getUnits().size(), MAX_UNITS_COUNT) / MAX_UNITS_COUNT;
			
			if(probability >= PRNG.nextFloat(1.0f)) {
				
				Vec2f position = new Vec2f(
						planet.getPosition().getX()  + PRNG.nextFloat(-1, 1) * planet.getRadius(),
						planet.getPosition().getY() + PRNG.nextFloat(-1, 1) * planet.getRadius()
				);
				
				executor.runAnimation(new ExplosionAnimation(position));
				
			}
			
		}
		
	}
	
}
