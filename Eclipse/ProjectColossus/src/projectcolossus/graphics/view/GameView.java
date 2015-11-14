package projectcolossus.graphics.view;

import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import projectcolossus.gamelogic.GameData;
import projectcolossus.gamelogic.GameMap;
import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.FreeCard;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.graphics.BackgroundRenderer;
import projectcolossus.graphics.Camera;
import projectcolossus.graphics.GameViewInputManager;
import projectcolossus.graphics.MultisampleConfigChooser;
import projectcolossus.graphics.PlanetRenderer;
import projectcolossus.graphics.RouteRenderer;
import projectcolossus.graphics.animation.Animation;
import projectcolossus.graphics.animation.AnimationExecutor;
import projectcolossus.graphics.animation.CameraAnimation;
import projectcolossus.graphics.animation.ExplosionGenerator;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.android.lib.opengl.GLMatrixStack;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

public class GameView extends GLSurfaceView implements GameViewInputManager.Listener {
	
	public static final int DEFAULT_CAMERA_Z = 300;
	
	// camera
	private Camera camera;
	private float viewWidth, viewHeight, ratio;
	
	// The owner activity
	private Activity owner;
	
	// the current game data
	private GameData gameData;
	
	// the current game map
	private GameMap gameMap;
	
	// Input management
	private GameViewInputManager inputManager;
	
	// matrices
	private GLMatrixStack viewStack, projStack;
	private float[] projInverse;
	
	// renderers
	private GameRenderer rndGame;
	private PlanetRenderer rndPlanet;
	private RouteRenderer rndRoute;
	private BackgroundRenderer rndBackground;
	
	// animations
	private AnimationExecutor animationExecutor;
	private ExplosionGenerator explosionGenerator;
	
	// planet info panel
	private PlanetView viewPlanet;
	
	// listeners
	private ArrayList<Listener> listeners;
	
	private boolean glSurfaceReady;
	
	public GameView(Context context, Activity owner, PlanetView planetInfoView, GameMap gameMap) {
		super(context);
		
		this.listeners = new ArrayList<Listener>();
		this.glSurfaceReady = false;
		this.rndGame = new GameRenderer();
		this.camera = new Camera(DEFAULT_CAMERA_Z);
		this.viewPlanet = planetInfoView;
		this.animationExecutor = new AnimationExecutor();
		this.explosionGenerator = new ExplosionGenerator(animationExecutor);
		this.gameMap = gameMap;
		this.owner = owner;
		
		setEGLContextClientVersion(2);
		setEGLConfigChooser(5, 6, 5, 0, 24, 0);
		setRenderer(rndGame);
		setRenderMode(RENDERMODE_CONTINUOUSLY);
		setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR);

		// input and camera management
		inputManager = new GameViewInputManager(camera);
		inputManager.setMapSize(gameMap.getSize());
		inputManager.addListener(this);		

		setOnTouchListener(inputManager);
		setOnDragListener(inputManager);		
		
	}
	
	public boolean addListener(Listener l) { return listeners.add(l); }
	public boolean removeListener(Listener l) { return listeners.remove(l); }
	
	public AnimationExecutor getAnimationExecutor() { return animationExecutor; }
	
	public void update(Player currentPlayer, GameData gameData) {
		if(gameData != null) {
			this.gameData = gameData;
			this.gameMap = gameData.getGameMap();
		}
		if(glSurfaceReady)
			queueEvent(new UpdateRunnable(currentPlayer, gameData));
	}
	
	
	@Override
	public boolean onCardDropped(CardLittleView cardView, Vec2f pos) {
		
		int index = cardView.getIndex();
		Card card = cardView.getCard();
		
		
		if(card instanceof FreeCard) {
			fireOnFreeCardDropped((FreeCard)card, index);
			return true;
		} else  if(card instanceof PlanetCard) {
			Planet p = planetAt(pos);
			
			if(p != null) {
				fireOnPlanetCardDropped((PlanetCard)card, index, p.getID());
				return true;
			}
			
			return false;
		}
		
		return false;
	}
	
	@Override
	public void onClick(Vec2f pos) {
		final Planet clickedPlanet = planetAt(pos);
		
		if(clickedPlanet != null)
			fireOnPlanetClicked(clickedPlanet.getID());
		else
			fireOnPlanetClicked(0);
		
	}

	
	public boolean gotoPlanet(int planetID) {
		Planet planet = gameData.getPlanetByID(planetID);
		
		if(planet != null) {
			Vec3f endCameraPos = new Vec3f(planet.getPosition().getX(), planet.getPosition().getY(), camera.getZ());
			
			CameraAnimation animCamera = new CameraAnimation(inputManager, 0.5f, camera, endCameraPos);
			animationExecutor.runAnimation(animCamera);
			
			return true;	
		}
		
		return false;
	}
	
    private Planet planetAt(Vec2f position) {
        for(Planet p : gameMap.getPlanets()) {
            float distance = position.substract(p.getPosition()).length();
            if(distance < p.getRadius())
                return p;
        }
        return null;
    } 

    public boolean isReady() {
    	return glSurfaceReady;
    }
    
    public void waitLoaded() {
    	while(!isReady())
    		Thread.yield();	
    }
    
    protected void fireOnPlanetCardDropped(PlanetCard card, int cardIndex, int planetID) {
    	for(Listener l : listeners)
    		l.onPlanetCardDropped(card, cardIndex, planetID);
    }
    
    protected void fireOnFreeCardDropped(FreeCard card, int cardIndex) {
    	for(Listener l : listeners)
    		l.onFreeCardDropped(card, cardIndex);
    }
    
    
    protected void fireOnPlanetClicked(int planetID) {
    	for(Listener l : listeners)
    		l.onPlanetClicked(planetID);
    }
    
    
    public static interface Listener {
    	public void onPlanetClicked(int planetID);
    	public void onFreeCardDropped(FreeCard card, int cardIndex);
    	public void onPlanetCardDropped(PlanetCard card, int cardIndex, int planetID);
    }
	
    
    private class UpdateRunnable implements Runnable {
    	
    	private GameData gameData;
    	private Player player;
    	
    	public UpdateRunnable(Player player, GameData gameData) {
    		this.player = player;
    		this.gameData = gameData;
    	}
    	
    	public void run() {
    		
    		List<Planet> visiblePlanets = gameData.getVisiblePlanets(player);
    		
    		explosionGenerator.setVisiblePlanets(visiblePlanets);
			rndPlanet.setGameData(this.gameData);
			rndPlanet.setVisiblePlanets(visiblePlanets);
    	}
    	
    }
    
	private class GameRenderer implements GLSurfaceView.Renderer {
		
		@Override
		public void onDrawFrame(GL10 gl) {
			
			float[] vpMatrix = new float[16];
			
			explosionGenerator.generateExplosions();
			
			animationExecutor.update();
			
			viewStack.setIdentity();
			viewStack.translate(-camera.getX(), -camera.getY(), -camera.getZ());
			
			Matrix.multiplyMM(vpMatrix, 0, projStack.current(), 0, viewStack.current(), 0);
			
			GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
			
			// Draw background
			rndBackground.drawStars(vpMatrix);
	
			// Draw routes
			rndRoute.drawRoutes(vpMatrix);
			
			// Draw planets
			rndPlanet.drawPlanets(vpMatrix);
			
			// Draw animations
			animationExecutor.draw(vpMatrix);
			
			// Draw planet icons
			rndPlanet.drawPlanetIcons(vpMatrix);
			
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GLES20.glViewport(0, 0, width, height);		
			
			viewWidth = width;
			viewHeight = height;
			ratio = viewWidth / viewHeight;
			
			projStack.setPerspective(45.0f, ratio, 250.0f, 550.0f);

			inputManager.updateCameraMetrics(projStack.current(), width, height);
			inputManager.updateFromCamera();
			
			Matrix.invertM(projInverse, 0, projStack.current(), 0);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// Init opengl
			GLES20.glClearColor(0, 0, 0, 0);
			GLES20.glEnable(GLES20.GL_DEPTH_TEST);
			GLES20.glEnable(GLES20.GL_CULL_FACE);		
			GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
			
			int iocan[] = new int[1];
			GLES20.glGetIntegerv(GLES20.GL_STENCIL_BITS, iocan, 0);
			Log.d("GLInfo", "" + iocan[0]);
			
			// matrices
			viewStack = new GLMatrixStack(16);
			projStack = new GLMatrixStack(4);
			projInverse = new float[16];
			
			// load resources
			ResourceLoader.loadPlanetFXs();
			ResourceLoader.loadPrograms();
			ResourceLoader.loadCommonVBOs();
			ResourceLoader.loadTextures();
	
			// renderes
			rndPlanet = new PlanetRenderer(getResources(), gameMap);
			rndRoute = new RouteRenderer(getResources(), gameMap);	
			rndBackground = new BackgroundRenderer(getResources(), gameMap.getSize());
			
			glSurfaceReady = true;
		}
	}
}
