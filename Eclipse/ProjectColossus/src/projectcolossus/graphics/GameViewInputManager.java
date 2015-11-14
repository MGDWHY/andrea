package projectcolossus.graphics;

import java.util.ArrayList;

import projectcolossus.gamelogic.Planet;
import projectcolossus.gamelogic.Vec2f;
import projectcolossus.gamelogic.cards.Card;
import projectcolossus.gamelogic.cards.FreeCard;
import projectcolossus.gamelogic.cards.PlanetCard;
import projectcolossus.graphics.view.CardLittleView;
import andrea.bucaletti.android.lib.vecmath.Vec3f;
import andrea.bucaletti.android.lib.vecmath.Vec4f;
import android.graphics.RectF;
import android.opengl.Matrix;
import android.util.Log;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.View;

public class GameViewInputManager implements View.OnTouchListener, View.OnDragListener {
	
	public static final float MAP_EDGE_GAP = 100;
	
	private Camera camera;
	
	private float translateX, translateY;
	
	private RectF rCameraSpace; // max space;
	private Vec3f npTopLeft, npTopRight, npBottomRight, npBottomLeft; // near plane
	private Vec3f vTopLeft, vTopRight, vBottomRight, vBottomLeft; // projection vector
	private Vec3f pTopLeft, pTopRight, pBottomRight, pBottomLeft; // projected plane
	
	private float[] matProjInverse;
	
	private float viewWidth, viewHeight;
	
	private Vec2f startTouchCoords, prevTouchCoords;
	
	private ArrayList<Listener> listeners;
	
	private boolean inputEnabled;
	
	public GameViewInputManager(Camera camera) {
		this.camera = camera;
		this.matProjInverse = new float[16];
		this.rCameraSpace = new RectF();
		this.prevTouchCoords = new Vec2f();
		this.startTouchCoords = new Vec2f();
		this.listeners = new ArrayList<Listener>();
		this.inputEnabled = true;
	}
	
	public void setMapSize(Vec2f mapSize) {
		this.rCameraSpace = new RectF(-MAP_EDGE_GAP, mapSize.getY() + MAP_EDGE_GAP, mapSize.getX() + MAP_EDGE_GAP, -MAP_EDGE_GAP);
	}
	
	@Override
	public boolean onDrag(View v, DragEvent evt) {
		Object obj = evt.getLocalState();
		
		switch(evt.getAction()) {
		case DragEvent.ACTION_DROP:
			Vec2f touchCoords = new Vec2f(evt.getX(), evt.getY());
			Vec2f mapCoords = getMapCoords(touchCoords);	
			CardLittleView cardView = (CardLittleView) obj;
			return fireOnCardDropped(cardView, mapCoords);
		}
		return true;		
	}
	
	@Override
	public boolean onTouch(View v, MotionEvent evt) {
		
		if(!inputEnabled)
			return true;
		
		switch(evt.getAction()) {
		case MotionEvent.ACTION_DOWN:
			prevTouchCoords = getTouchCoords(evt);
			startTouchCoords = getTouchCoords(evt);
			return true;
		case MotionEvent.ACTION_MOVE:
			
			Vec2f touchCoords = getTouchCoords(evt);
			
			Vec2f coords = getNCSCoords(prevTouchCoords)
				.substract(getNCSCoords(touchCoords));
					
			translateCamera(coords);
			
			prevTouchCoords = touchCoords;
			
			return true;
		case MotionEvent.ACTION_UP:
			prevTouchCoords = getTouchCoords(evt);
			
			if(startTouchCoords.substract(prevTouchCoords).length() < 10) // click
				fireOnClick(getMapCoords(prevTouchCoords));
			
			return true;
		}
		
		return false;
	}	
	
	public void addListener(Listener l) { if(l != null) listeners.add(l); }
	public void removeListener(Listener l) { listeners.remove(l); }
	
	public void setInputEnabled(boolean enabled) {
		inputEnabled = enabled;
	}
	
	public void updateCameraMetrics(float[] projectionMatrix, float viewWidth, float viewHeight) {
		
		Vec4f temp = new Vec4f();
		
		this.viewWidth = viewWidth;
		this.viewHeight = viewHeight;
		
		Matrix.invertM(this.matProjInverse, 0, projectionMatrix, 0);
		
		// top left
		Matrix.multiplyMV(temp.getComponents(), 0, matProjInverse, 0, new Vec4f(-1, 1, -1, 1).getComponents(), 0);
		npTopLeft = temp.toEuclideanVector();
		vTopLeft = npTopLeft.normalize();
		
		// top right
		Matrix.multiplyMV(temp.getComponents(), 0, matProjInverse, 0, new Vec4f(1, 1, -1, 1).getComponents(), 0);
		npTopRight = temp.toEuclideanVector();
		vTopRight = npTopRight.normalize();
		
		// bottom right
		Matrix.multiplyMV(temp.getComponents(), 0, matProjInverse, 0, new Vec4f(1, -1, -1, 1).getComponents(), 0);
		npBottomRight = temp.toEuclideanVector();
		vBottomRight = npBottomRight.normalize();
		
		// bottom left
		Matrix.multiplyMV(temp.getComponents(), 0, matProjInverse, 0, new Vec4f(-1, -1, -1, 1).getComponents(), 0);
		npBottomLeft = temp.toEuclideanVector();
		vBottomLeft = npBottomLeft.normalize();
	}
	
	public void updateFromCamera() {

		float t = -(npTopLeft.z + camera.getZ()) / vTopLeft.z;
		
		pTopLeft = npTopLeft.add(vTopLeft.scale(t));
		pTopRight = npTopRight.add(vTopRight.scale(t));
		pBottomRight = npBottomRight.add(vBottomRight.scale(t));
		pBottomLeft = npBottomLeft.add(vBottomLeft.scale(t));

		this.translateX = camera.getX() - getCameraHorizonWidth() / 2.0f; 		
		
		this.translateY = camera.getY() - getCameraHorizonHeight() / 2.0f; 
	}
	
	public float getCameraHorizonWidth() {
		return (pTopRight.x - pTopLeft.x);
	}
	
	public float getCameraHorizonHeight() {
		return (pTopRight.y - pBottomRight.y);
	}
	
	public float pixelsToUnits(int pixels) {
		return getCameraHorizonWidth() * pixels / viewWidth;
	}
	
	private Vec2f getTouchCoords(MotionEvent evt) {
		return new Vec2f(evt.getX(), evt.getY());
	}

	private Vec2f getNCSCoords(Vec2f touchCoords) {
		return new Vec2f(
			(touchCoords.getX() / viewWidth) * 2 - 1,
			-((touchCoords.getY() / viewHeight) * 2 - 1)
		);
	}
	
	private Vec2f getMapCoords(Vec2f touchCoords) {
		float x = touchCoords.getX() / viewWidth * getCameraHorizonWidth();
		float y = (viewHeight - touchCoords.getY()) / viewHeight * getCameraHorizonHeight();
		return new Vec2f(translateX + x, translateY + y);
	}
	
	private void translateCamera(Vec2f t) {
		translateCamera(t.getX(), t.getY());
	}
	
	private void translateCamera(float stepX, float stepY) {
		translateX += (stepX / 2.0f) * getCameraHorizonWidth();
		translateY += (stepY / 2.0f) * getCameraHorizonHeight();
		
		if(translateX < rCameraSpace.left)
			translateX = rCameraSpace.left;
		
		if(translateY < rCameraSpace.bottom)
			translateY = rCameraSpace.bottom;
		
		if(translateX > rCameraSpace.right - getCameraHorizonWidth())
			translateX = rCameraSpace.right - getCameraHorizonWidth();
		
		if(translateY > rCameraSpace.top - getCameraHorizonHeight())
			translateY = rCameraSpace.top - getCameraHorizonHeight();
		
		camera.setX(translateX + getCameraHorizonWidth() / 2.0f);
		camera.setY(translateY + getCameraHorizonHeight() / 2.0f);
	}	
	
	private void fireOnClick(Vec2f pos) {
		for(Listener l : listeners)
			l.onClick(pos);
	}
	
	private boolean fireOnCardDropped(CardLittleView cardView, Vec2f pos) {
		boolean result = true;
		for(Listener l : listeners)
			result = result && l.onCardDropped(cardView, pos);
		return result;
	}
	
	public static interface Listener {
		public void onClick(Vec2f pos);
		public boolean onCardDropped(CardLittleView cardView, Vec2f pos);
	}
}
