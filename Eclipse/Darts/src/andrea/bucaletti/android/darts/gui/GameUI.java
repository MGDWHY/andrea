package andrea.bucaletti.android.darts.gui;

import andrea.bucaletti.android.darts.lib.Constants;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.g2d.Rectangle;

import andrea.bucaletti.darts.R;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class GameUI implements OnTouchListener {
	
	
	public static enum ShootingState { Ready, Shooting, Shooted };

	public static interface Listener { // listener for this UI
		public void Shoot(float force);
	}		
	
	public static final float WIDTH = 100; // scaled width default
	
	public static final float PADDING = 5; // ui padding
	
	public static final float BTN_READY_SIZE = 15; // scaled ready button size
	public static final float FORCE_BAR_WIDTH = 40; // force bar width
	public static final float FORCE_BAR_HEIGHT = 5; // force bar height
	
	public static final float TOUCH_ANGLE_SCALING = 45; // angle rotation scaling
	
	// force bar colors
	public static final float FORCE_BAR_COLOR1[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	public static final float FORCE_BAR_COLOR2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	public static final float FORCE_BAR_BORDER_COLOR[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	public static final float FORCE_BAR_BACK_COLOR[] = { 0.0f, 0.0f, 0.0f, 0.4f };
	
	// force bar parameters
	public static final float MAX_FORCE = 100;
	public static final float FORCE_SPEED = MAX_FORCE / 1.5f;
	
	
	private Context context;
	
	// UI size
	private float width, height;
	private float scaledWidth, scaledHeight;
	
	private float[] projectionMatrix;
	
	// ready/shoot button
	private int buttonReadyTexID; // ready  button texture
	private int buttonShootTexID; // shoot button texture
	private int buttonTexID; // current texture (ready or shoot)
	private boolean buttonVisible;
	private Rectangle buttonReady; // ready button square	

	
	// force bar
	private ForceBar forceBar;
	private Rectangle forceBarBorder;
	private boolean forceBarVisible;
	
	// crossair
	private Crossair crossair;
	
	// programs
	private int textureProgram;
	private int colorProgram;
	private int gradientProgram;
	
	// force bar values
	private float force;
	private boolean forceIncreasing;
	
	private float rotateY; // current X-axis angle
	private ShootingState shootingState = ShootingState.Ready; // current state of shooting	
	
	
	// Touch event listener valiables
	private boolean isDragging = false; // user is dragging
	private boolean buttonDown = false; // user is tapping the button
	private float prevTouchX;
	
	// Thouch event registered listener
	private Listener listener;
	
	
	public GameUI(Context context) {
		
		this.context = context;
		
		projectionMatrix = new float[16];
		
		rotateY = 0;
		
		buttonVisible = true;
		
		force = 0;
		forceIncreasing = true;
		forceBarVisible = false;
		
		Matrix.setIdentityM(projectionMatrix, 0);
	}
	
	public void initilize() { // to be called after GL surface has been created
		buttonTexID = buttonReadyTexID = GLU.loadTexture2D(R.drawable.ready0, context.getResources());
		buttonShootTexID = GLU.loadTexture2D(R.drawable.shoot0, context.getResources());
		
		textureProgram = GLU.loadProgram(R.string.gameui_2d_vs, R.string.gameui_2dtexture_fs, context.getResources());
		colorProgram = GLU.loadProgram(R.string.gameui_2d_vs, R.string.gameui_2dcolor_fs, context.getResources());
		gradientProgram = GLU.loadProgram(R.string.gameui_2d_vs, R.string.gameui_2dhgradient_fs, context.getResources());
	}
	
	public void setListener(Listener l) {
		this.listener = l;
	}
	
	public void setViewSize(float w, float h) {
		
		float ratio = w / h;
		
		width = w;
		height = h;
		
		scaledWidth = WIDTH;
		scaledHeight =  scaledWidth / ratio;
		
		buttonReady = new Rectangle(scaledWidth - BTN_READY_SIZE, scaledHeight - BTN_READY_SIZE, BTN_READY_SIZE, BTN_READY_SIZE);
		forceBar = new ForceBar(scaledWidth / 2 - FORCE_BAR_WIDTH / 2, scaledHeight - FORCE_BAR_HEIGHT, 
				FORCE_BAR_WIDTH, FORCE_BAR_HEIGHT);
		
		forceBarBorder = new Rectangle(forceBar);
		
		crossair = new Crossair(scaledWidth / 2, scaledHeight / 2, Constants.UI_CROSSAIR_SIZE);
		
		Matrix.orthoM(projectionMatrix, 0, -PADDING / ratio, scaledWidth + PADDING / ratio, -PADDING, scaledHeight + PADDING, -1, 1);		
	}
	
	public void update(float dt) {
		if(forceIncreasing) {
			force += FORCE_SPEED * dt;
			if(force >= MAX_FORCE) {
				force = MAX_FORCE;
				forceIncreasing = false;
			}
		} else {
			force -= FORCE_SPEED * dt;
			if(force <= 0) {
				force = 0;
				forceIncreasing = true;
			}
		}
		
		forceBar.setForce(force);
	}
	
	public void draw() {
		
		// Uniforms
		int projectionMatrixHandle, textureHandle, color1Handle, color2Handle;
		
		// Attributes
		int  positionHandle, texCoordHandle;
		
		/* Enable blending and disable depth test */
		GLES20.glDisable(GLES20.GL_DEPTH_TEST);
		GLES20.glEnable(GLES20.GL_BLEND);		
		
		/* Draw ready/shoot button */		
		
		projectionMatrixHandle = GLES20.glGetUniformLocation(textureProgram, "in_ProjectionMatrix");
		textureHandle = GLES20.glGetUniformLocation(textureProgram, "in_Texture");
		
		positionHandle = GLES20.glGetAttribLocation(textureProgram, "in_Position");
		texCoordHandle = GLES20.glGetAttribLocation(textureProgram, "in_TexCoord");
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		
		GLES20.glUseProgram(textureProgram);
			
			if(buttonVisible) {
				GLES20.glUniformMatrix4fv(projectionMatrixHandle, 1, false, projectionMatrix, 0);
				GLES20.glUniform1i(textureHandle, 0);
				
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, buttonTexID);
				buttonReady.drawIndexed(positionHandle, texCoordHandle);
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
			}

			
		GLES20.glUseProgram(0);
		

		if(forceBarVisible) {
			/* Draw force Bar back */
			
			positionHandle = GLES20.glGetAttribLocation(colorProgram, "in_Position");
			texCoordHandle = GLES20.glGetAttribLocation(colorProgram, "in_TexCoord");
			
			projectionMatrixHandle = GLES20.glGetUniformLocation(colorProgram, "in_ProjectionMatrix");
			color1Handle = GLES20.glGetUniformLocation(colorProgram, "in_Color");
			
			GLES20.glUseProgram(colorProgram);
			
				GLES20.glUniformMatrix4fv(projectionMatrixHandle, 1, false, projectionMatrix, 0);		
				GLES20.glUniform4fv(color1Handle, 1, FORCE_BAR_BACK_COLOR, 0);
				
				forceBarBorder.drawIndexed(positionHandle, texCoordHandle);
			
			GLES20.glUseProgram(0);		
	
			/* Draw force bar */		
			
			positionHandle = GLES20.glGetAttribLocation(gradientProgram, "in_Position");
			texCoordHandle = GLES20.glGetAttribLocation(gradientProgram, "in_TexCoord");
			
			projectionMatrixHandle = GLES20.glGetUniformLocation(gradientProgram, "in_ProjectionMatrix");
			color1Handle = GLES20.glGetUniformLocation(gradientProgram, "in_Color1");
			color2Handle = GLES20.glGetUniformLocation(gradientProgram, "in_Color2");
			
			GLES20.glUseProgram(gradientProgram);
	
				GLES20.glUniformMatrix4fv(projectionMatrixHandle, 1, false, projectionMatrix, 0);		
				GLES20.glUniform4fv(color1Handle, 1, FORCE_BAR_COLOR1, 0);
				GLES20.glUniform4fv(color2Handle, 1, FORCE_BAR_COLOR2, 0);		
			
				forceBar.drawIndexed(positionHandle, texCoordHandle);
			
			GLES20.glUseProgram(0);
			
			/* Draw force Bar border */
			
			positionHandle = GLES20.glGetAttribLocation(colorProgram, "in_Position");
			texCoordHandle = GLES20.glGetAttribLocation(colorProgram, "in_TexCoord");
			
			projectionMatrixHandle = GLES20.glGetUniformLocation(colorProgram, "in_ProjectionMatrix");
			color1Handle = GLES20.glGetUniformLocation(colorProgram, "in_Color");
			
			GLES20.glLineWidth(1);
			
			GLES20.glUseProgram(colorProgram);
			
				GLES20.glUniformMatrix4fv(projectionMatrixHandle, 1, false, projectionMatrix, 0);		
				GLES20.glUniform4fv(color1Handle, 1, FORCE_BAR_BORDER_COLOR, 0);
				
				forceBarBorder.drawArrays(positionHandle, texCoordHandle, GLES20.GL_LINE_LOOP);
			
			GLES20.glUseProgram(0);
		}
		
		/* Draw crossair */
		
		GLES20.glLineWidth(2);  
		
		GLES20.glUseProgram(colorProgram);
			projectionMatrixHandle = GLES20.glGetUniformLocation(colorProgram, "in_ProjectionMatrix");
			color1Handle = GLES20.glGetUniformLocation(colorProgram, "in_Color");
			
			positionHandle = GLES20.glGetAttribLocation(colorProgram, "in_Position");
			
			GLES20.glUniformMatrix4fv(projectionMatrixHandle, 1, false, projectionMatrix, 0);
			GLES20.glUniform4fv(color1Handle, 1, Constants.UI_CROSSAIR_COLOR, 0);
			
			crossair.drawArrays(positionHandle, -1, GLES20.GL_LINES);
		GLES20.glUseProgram(0);
		
		GLU.openGLError();
	}
	
	public float getRotateY() {
		return rotateY;
	}
	
	public ShootingState getShootingState() {
		return shootingState;
	}
	
	public void setReady() { // resets the shooting state to ready
		shootingState = ShootingState.Ready;
		buttonTexID = buttonReadyTexID;
		buttonVisible = true;
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		
		float x = event.getX();
		float y = event.getY();
		
		float scaledX = x / width * scaledWidth;
		float scaledY = scaledHeight - (y / height * scaledHeight);

		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			if(buttonReady.containsPoint(scaledX, scaledY)) {
				buttonDown = true;
			} else {
				prevTouchX = event.getX();
				isDragging = true;
			}
			
			return true;
		case MotionEvent.ACTION_MOVE:
			if(isDragging) {
				float movement = (event.getX() - prevTouchX) / width; // between -1 and 1
				prevTouchX = event.getX();
				rotateY -= movement * TOUCH_ANGLE_SCALING; // [-1, 1] * base_angle
			}
			return true;
		case MotionEvent.ACTION_UP:
			
			if(buttonDown && buttonReady.containsPoint(scaledX, scaledY))
				readyShootButtonClick();
			
			isDragging = false;
			buttonDown = false;
			return true;
		default:
			return false;
		}
	}
	
	protected void fireShoot(float force) {
		if(listener != null)
			listener.Shoot(force);
	}
	
	private void readyShootButtonClick() {
		Log.d("Test3", "Shooting State: " + shootingState);
		if(shootingState == ShootingState.Ready) { // first click
			buttonTexID = buttonShootTexID;
			shootingState = ShootingState.Shooting;
			forceBarVisible = true;
			force = 0;
			forceIncreasing = true;
		} else if(shootingState == ShootingState.Shooting) {
			shootingState = ShootingState.Shooted;
			buttonVisible = false;
			forceBarVisible = false;
			fireShoot(force);
		}
	}
	

}
