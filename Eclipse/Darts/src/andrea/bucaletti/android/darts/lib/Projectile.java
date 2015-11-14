package andrea.bucaletti.android.darts.lib;

import andrea.bucaletti.android.lib.vecmath.Vec3f;

public class Projectile {
	
	public static class Parameters {
		public Vec3f StartPosition;
		public Vec3f Direction;
		public Vec3f WindDirection;
		public float Force;
		public float WindForce;
	}
	
	public static class Modifiers {
		
		public float GravityFactor;
		public float WindFactor;
		public float ForceFactor;
		public float TimeFactor;
		
		public Modifiers() {
			GravityFactor = WindFactor = ForceFactor = TimeFactor = 1.0f; // default;
		}
	}	
	
	private Vec3f position;
	
	private Vec3f velocity;
	
	private Vec3f finalGravity, finalWind;
	
	private float timeFactor;
	
	
	public Projectile(Parameters params, Modifiers mods) {

		this.position = new Vec3f(params.StartPosition);
		
		this.velocity = params.Direction.scale(params.Force * Constants.P_FORCE_VELOCITY_SCALING);
		
		this.finalGravity = Constants.P_GRAVITY_VECTOR.scale(mods.GravityFactor * Constants.P_GRAVITY);
		
		this.finalWind = params.WindDirection.scale(params.WindForce * mods.WindFactor);
		
		this.timeFactor = mods.TimeFactor;
	}
	
	public void update(float dt) {
		
		dt *= timeFactor;
		
		/* add gravity
		 * velocity = velocity + gravity * t;
		 */
		velocity = velocity.add(finalGravity.scale(dt));
		
		/*
		 * calculate position
		 * position = position + velocity * t + wind * t;
		 */
		position = position.add(velocity.scale(dt)).add(finalWind.scale(dt));
		
	}
	
	public Vec3f getPosition() {
		return position;
	}
	

}
