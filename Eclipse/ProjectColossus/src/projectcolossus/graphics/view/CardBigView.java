package projectcolossus.graphics.view;

import java.util.Arrays;

import projectcolossus.gamelogic.cards.Card;
import projectcolossus.res.CardTemplate;
import projectcolossus.res.ResourceLoader;
import andrea.bucaletti.projectcolossus.R;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.shapes.OvalShape;
import android.graphics.drawable.shapes.RectShape;
import android.graphics.drawable.shapes.RoundRectShape;
import android.text.Html;
import android.util.TypedValue;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class CardBigView extends RelativeLayout {
	
	public final float width, height;
	
	protected Activity owner;
	
	protected float border, innerBorder;
	
	protected float logoSize;
	
	protected float cardImgWidth, cardImgHeight;
	
	protected float titleSize, textSize;
	
	protected Card card;
	protected CardTemplate template;
	
	protected Paint pntFill, pntStroke, pntText;
	protected RoundRectShape shpBorder;	
	protected RectShape shpBg;
	protected OvalShape shpResources;
	
	protected Bitmap cardKindLogo;
	
	protected int borderColor, bgColor;
	
	protected TextView txtCardDescription;
	
	private Rect r1, r2;

	public CardBigView(Context context, Activity owner, Card card) {
		super(context);
		
		float[] corners = new float[8];
		
		this.owner = owner;
		
		this.width = getResources().getDimension(R.dimen.card_big_width);
		this.height = getResources().getDimension(R.dimen.card_big_height);
		this.border = getResources().getDimension(R.dimen.card_big_border);
		this.logoSize = getResources().getDimension(R.dimen.card_big_logo_size);
		this.cardImgWidth = getResources().getDimension(R.dimen.card_big_img_width);
		this.cardImgHeight = getResources().getDimension(R.dimen.card_big_img_height);
		this.titleSize = getResources().getDimension(R.dimen.card_big_title_size);
		this.textSize = getResources().getDimension(R.dimen.card_big_text_size);
		this.innerBorder = border / 4;
		
		this.txtCardDescription = new TextView(context);

		Arrays.fill(corners, border);
		
		this.shpBorder = new RoundRectShape(corners, null, null);
		this.shpBg = new RectShape();
		this.shpResources = new OvalShape();
		this.r1 = new Rect();
		this.r2 = new Rect();
		
		this.pntFill = new Paint();
		this.pntStroke = new Paint();
		this.pntText = new Paint();

		
		shpBorder.resize(width, height);
		shpBg.resize(width - border * 2, height - border * 2);
		shpResources.resize(titleSize, titleSize);
		
		pntFill.setStrokeWidth(0);
		pntFill.setStyle(Paint.Style.FILL);
		
		pntStroke.setStrokeWidth(innerBorder);
		pntStroke.setStyle(Paint.Style.STROKE);
		
		pntText.setStyle(Paint.Style.STROKE);
		pntText.setColor(getResources().getColor(R.color.black));
		pntText.setTextAlign(Paint.Align.LEFT);
		

		RelativeLayout.LayoutParams params;		
		
		// Description text
		addView(txtCardDescription);
		txtCardDescription.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
		txtCardDescription.setTextColor(getResources().getColor(R.color.black));
		
		params = (RelativeLayout.LayoutParams)txtCardDescription.getLayoutParams();
		params.width = (int)cardImgWidth;
		params.height = LayoutParams.WRAP_CONTENT;
		params.setMargins((int) border * 3, (int) (border * 5 + cardImgHeight), 0, 0);
		
		
		setBackgroundColor(getResources().getColor(R.color.transparent));
		
		setCard(card);
	}
	
	public void setCard(Card card) {
		this.card = card;
		
		template = ResourceLoader.getCardTemplate(card.getID());
		
		borderColor = 0xff9F26BD;
		bgColor = 0xffDBDBDB;
		
		cardKindLogo = ResourceLoader.getCardKindTinyLogoMasked(card.getKind(), borderColor);
		
		txtCardDescription.setText(Html.fromHtml(template.getDescription()));
		
		postInvalidate();
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		canvas.save();
		
			// background
			pntFill.setColor(borderColor);
			shpBorder.draw(canvas, pntFill);
			
			canvas.translate(border, border);
			pntFill.setColor(bgColor);
			shpBg.draw(canvas, pntFill);
			
			
			canvas.save();
				// card Image
				pntStroke.setColor(borderColor);
				canvas.translate(border * 2, border * 3);
				
				//drawBitmap(canvas, template.getLargeImage(), (int)cardImgWidth, (int)cardImgHeight);
				template.getLargeImage().setBounds(0, 0, (int)cardImgWidth, (int)cardImgHeight);
				template.getLargeImage().draw(canvas);
				canvas.drawRect(0,  0, cardImgWidth, cardImgHeight, pntStroke);
				
			canvas.restore();
			
			canvas.save();
				// card name
				canvas.save();
					pntText.setTextSize(titleSize);
					canvas.translate(border + logoSize + pntText.descent(), 
							border + titleSize - pntText.descent());
					canvas.drawText(card.getName(), 0, 0, pntText);		
				canvas.restore();
				
				// card resources
				canvas.save();
					canvas.translate(border * 2 + cardImgWidth - titleSize, border);
					pntFill.setColor(borderColor);
					
					for(int i = 0; i < card.getResourceCost(); i++) {
						shpResources.draw(canvas, pntFill);
						canvas.translate(-titleSize, 0);
					}
				
				canvas.restore();
				
				
			canvas.restore();
			
			// card logo
			
			pntFill.setColor(bgColor);
			pntStroke.setColor(borderColor);
			
			canvas.translate(border, border);
			canvas.drawRect(0, 0, logoSize, logoSize, pntFill);
			drawBitmap(canvas, cardKindLogo, (int)logoSize, (int)logoSize);
			canvas.drawRect(0, 0, logoSize, logoSize, pntStroke);
			
		canvas.restore();
		
	}
	
	private void drawBitmap(Canvas canvas, Bitmap bitmap, int width, int height) {
		r1.right = bitmap.getWidth();
		r1.bottom = bitmap.getHeight();
		
		r2.right = width;
		r2.bottom = height;
		
		canvas.drawBitmap(bitmap, r1, r2, null);
	}
	
	
}
