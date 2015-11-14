package projectcolossus.graphics.view.planetview;

import java.util.ArrayList;

import projectcolossus.gamelogic.Player;
import projectcolossus.gamelogic.units.Unit;
import projectcolossus.res.ResourceLoader;
import projectcolossus.util.Util;
import andrea.bucaletti.projectcolossus.R;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.graphics.drawable.shapes.RoundRectShape;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.TextView;

public class UnitsView extends GridView {
	
	public static final int CELL_SIZE = 50;
	public static final int NUM_COLUMNS = 4;
	public static final int SPACING = 10;
	
	protected final int cellSize;
	

	
	protected int selectedIndex;
	
	protected UnitAdapter adapter;
	
	public UnitsView(Context context) {
		super(context);
		
		float density = context.getResources().getDisplayMetrics().density;
		
		selectedIndex = -1;
		adapter = new UnitAdapter();
		cellSize = Util.pixelToDP(CELL_SIZE, density);
		
		setGravity(Gravity.CENTER_HORIZONTAL);
		setColumnWidth(Util.pixelToDP(cellSize, density));
		setNumColumns(NUM_COLUMNS);
		setHorizontalSpacing(Util.pixelToDP(SPACING, density));
		setVerticalSpacing(Util.pixelToDP(SPACING, density));
		
		setAdapter(adapter);
	}
	
	public void setSelectedIndex(int index) {
		if(index != selectedIndex) {
			this.selectedIndex =  index;
			adapter.notifyDataSetChanged();
		}
	}
	
	public void setUnits(Player player, ArrayList<Unit> units) {
		adapter.setUnits(units);
		selectedIndex = -1;
	}
	
	private class UnitAdapter extends BaseAdapter {
		
		protected ArrayList<Unit> units;
		
		public UnitAdapter() {
			this.units = new ArrayList<Unit>();
		}
		
		public UnitAdapter(ArrayList<Unit> units) {
			this.units = units;
		}
		
		public void setUnits(ArrayList<Unit> units) {
			this.units = units;
		}
		
		@Override
		public int getCount() {
			return units.size();
		}

		@Override
		public Object getItem(int position) {
			return units.get(position);
		}

		@Override
		public long getItemId(int position) {
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			Cell v;
			Unit u = units.get(position);
			if(convertView == null) {
				v = new Cell(getContext());
				v.setLayoutParams(new GridView.LayoutParams(cellSize, cellSize));
			} else {
				v = (Cell) convertView;
			}
			
			boolean selected = position == selectedIndex;
			
			v.setUnit(u, selected);
			
			return v;
		}
	}
	
	private static class Cell extends View {
		
		private static final float INSET_PADDING = 10;
		
		private static final float MOVEMENT_CIRCLE_RADIUS = 8;
		private static final float MOVEMENT_CIRCLE_MARGIN = 16;
		
		private static float[] OUTER_CORNERS = new float[] {
				10, 10, 10, 10, 10, 10, 10, 10
		};			
		
		private static float[] INNER_CORNERS = new float[] {
				0, 0, 0, 0, 0, 0, 0, 0
		};
		
		private static RectF INSET = new RectF(INSET_PADDING, INSET_PADDING, INSET_PADDING, INSET_PADDING);		
		
		protected OvalShape drwMovement;
		protected RoundRectShape drwBackground;		
		
		protected boolean selected;
		
		protected Bitmap unitImage;
		protected Unit unit;
		
		protected Rect src;
		protected RectF dest;
		
		protected Paint playerColor;

		public Cell(Context context) {
			super(context);
	
			this.drwMovement = new OvalShape();
			this.drwMovement.resize(8, 8);
			
			this.drwBackground = new RoundRectShape(OUTER_CORNERS, INSET, INNER_CORNERS);
			
			this.src = new Rect();
			this.dest = new RectF();

			this.playerColor = new Paint();
			this.playerColor.setStrokeWidth(2);
		}
		
		public void setUnit(Unit unit) {
			this.setUnit(unit, false);
		}
		
		public void setUnit(Unit unit, boolean selected) {
			
			this.selected = selected;
			
			this.unit = unit;
			this.unitImage = ResourceLoader.getCardTinyImage(unit.getRefCardID());
				
			this.postInvalidate();
		}
		
		@Override
		protected void onDraw(Canvas canvas) {
			super.onDraw(canvas);
	
			playerColor.setColor(unit.getPlayer().getColor());
			drwBackground.resize(getWidth(), getHeight());
			src.set(0, 0, unitImage.getWidth(), unitImage.getHeight());
			dest.set(INSET_PADDING, INSET_PADDING, getWidth() - INSET_PADDING, getHeight() - INSET_PADDING);
			
			canvas.drawBitmap(unitImage, src, dest, null);
			
			if(selected)
				playerColor.setStyle(Paint.Style.FILL_AND_STROKE);
			else 		
				playerColor.setStyle(Paint.Style.STROKE);
			
			drwBackground.draw(canvas, playerColor);	
			
			canvas.translate(MOVEMENT_CIRCLE_MARGIN, getHeight() - MOVEMENT_CIRCLE_MARGIN - MOVEMENT_CIRCLE_RADIUS);
			
			playerColor.setStyle(Paint.Style.FILL);
			for(int movement = 0; movement < unit.getMovement(); movement++) {
				drwMovement.draw(canvas, playerColor);
				canvas.translate(8, 0);
			}
			
		}
		
	}

}
