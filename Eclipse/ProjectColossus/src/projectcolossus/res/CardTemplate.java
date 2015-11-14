package projectcolossus.res;

import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;

public class CardTemplate {
	
	private static final int I_LARGE_IMAGE = 0;
	private static final int I_DESCRIPTION = 1;
	
	private String description;
	private Drawable large;
	
	public CardTemplate(TypedArray array) {	
		this.large = array.getDrawable(I_LARGE_IMAGE);
		this.description = array.getString(I_DESCRIPTION);
	}
	
	public String getDescription() {
		return description;
	}
	
	public Drawable getLargeImage() {
		return large;
	}
}
