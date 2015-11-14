package projectcolossus.graphics.view;

import projectcolossus.gamelogic.cards.Card;
import projectcolossus.graphics.dialog.CardDialogFragment;
import andrea.bucaletti.projectcolossus.R;
import android.app.Activity;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class CardLittleView extends RelativeLayout implements View.OnTouchListener {
	
	private Activity owner;
	
	private int cardIndex;	
	
	private Card card;
	
	private TextView txtResourceCost;
	private TextView txtCardName;
	private ImageView imgCard;
	
	private CardLittleShadowBuilder shadowBuilder;
	
	private GestureDetector gestureDetector;
	
	public CardLittleView(Activity owner, int cardIndex, Card card) {
		super(owner.getApplicationContext());
		
		this.owner = owner;
		this.cardIndex = cardIndex;
		this.card = card;
		
		View.inflate(getContext(), R.layout.card_little, this);
		
		imgCard = (ImageView)findViewById(R.id.imgCard);
		txtResourceCost = (TextView) findViewById(R.id.txtResourceCost);
		txtCardName = (TextView) findViewById(R.id.txtCardName);
		shadowBuilder = new CardLittleShadowBuilder(this);
		
		txtResourceCost.setText(card.getResourceCost() + "");
		txtCardName.setText(card.getName());
		
		gestureDetector = new GestureDetector(owner, new CardLittleViewListener(this));
		
		setOnTouchListener(this);
	}
	
	public int getIndex() { return cardIndex; }
	public Card getCard() { return card; }

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		return gestureDetector.onTouchEvent(event);
	}	
	
	
	private class CardLittleViewListener extends GestureDetector.SimpleOnGestureListener {
		
		private CardLittleView refView;
		
		public CardLittleViewListener(CardLittleView refView) {
			this.refView = refView;
		}
		
		@Override
		public boolean onDown(MotionEvent e) {
			return true;
		}
		
		@Override
		public boolean onDoubleTap(MotionEvent e) {
			CardDialogFragment fragment = new CardDialogFragment();
			fragment.setCard(card);
			fragment.show(owner.getFragmentManager(), "CardDialogFragment");
			Log.d("GLInfo", "lolle");
			return true;			
		}
		
		@Override
		public void onLongPress(MotionEvent e) {
			startDrag(null, shadowBuilder, refView, 0);			
		}
		
	}
	
	private class CardLittleShadowBuilder extends View.DragShadowBuilder {

		private CardLittleView view;
		
		private Drawable shadow;
		
		public CardLittleShadowBuilder(CardLittleView view) {
			super(view);
			this.view = view;
			shadow = new ColorDrawable(Color.RED);
		}
		
		@Override
		public void onProvideShadowMetrics(Point size, Point touch) {
			int width = getView().getWidth();
			int height = getView().getHeight();
			
			size.set(width, height);
			touch.set(width / 2, height / 2);
			
			shadow.setBounds(0, 0, width, height);
		}
		
		@Override	
		public void onDrawShadow(Canvas canvas) {
			shadow.draw(canvas);
		}
	}
}
