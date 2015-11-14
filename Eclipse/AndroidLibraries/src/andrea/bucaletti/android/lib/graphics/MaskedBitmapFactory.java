package andrea.bucaletti.android.lib.graphics;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class MaskedBitmapFactory {
	
	/* red is taken as the new alpha */
	private static int ALPHA_MASK = 0x000000FF;
	
	private static int OVERLAY_MASK = 0x00FFFFFF;
	
	public static Bitmap decodeResource(Resources res, int resID, int overlayColor) {
		Bitmap src = BitmapFactory.decodeResource(res, resID);
		Bitmap dest = Bitmap.createBitmap(src.getWidth(), src.getHeight(), Bitmap.Config.ARGB_8888);
		
		int pixels[] = new int[src.getWidth() * src.getHeight()];
		
		src.getPixels(pixels, 0, src.getWidth(), 0, 0, src.getWidth(), src.getHeight());
		
		for(int i = 0; i < pixels.length; i++)
			pixels[i] = ((pixels[i] & ALPHA_MASK) << 24) | (overlayColor & OVERLAY_MASK);
		
		dest.setPixels(pixels, 0, dest.getWidth(), 0, 0, dest.getWidth(), dest.getHeight());
		
		src.recycle();
		
		return dest;
	}
	
}
