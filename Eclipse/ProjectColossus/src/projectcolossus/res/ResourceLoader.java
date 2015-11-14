package projectcolossus.res;

import java.io.IOException;
import java.util.HashMap;

import projectcolossus.gamelogic.Constants;
import projectcolossus.gamelogic.cards.heirs.AegisCard;
import projectcolossus.gamelogic.cards.heirs.GhostCard;
import projectcolossus.gamelogic.cards.heirs.HunterCard;
import projectcolossus.gamelogic.cards.heirs.MechaSquadCard;
import projectcolossus.gamelogic.cards.heirs.RaiderCard;
import projectcolossus.gamelogic.cards.heirs.ScoutCard;
import projectcolossus.graphics.fx.NoFX;
import projectcolossus.graphics.fx.PlanetFX;
import projectcolossus.graphics.fx.PlanetaryCageFX;
import projectcolossus.graphics.fx.ResourceLockFX;
import projectcolossus.util.Util;

import andrea.bucaletti.android.lib.graphics.MaskedBitmapFactory;
import andrea.bucaletti.android.lib.opengl.Android3DObject;
import andrea.bucaletti.android.lib.opengl.GLU;
import andrea.bucaletti.android.lib.opengl.GLVertexBufferObject;
import andrea.bucaletti.projectcolossus.R;
import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.media.AudioManager;
import android.media.SoundPool;
import android.opengl.GLES20;
import android.util.Log;
import android.util.SparseArray;
import android.util.SparseIntArray;

public class ResourceLoader {
	
	public static final String LOG_TAG = "ResourceLoader";
	
	private static Context context;
	private static Resources resources;
	private static float displayDensity;	
	
	private static SparseIntArray cardTinyImagesResID; 

	private static SparseArray<Bitmap> cardTinyImages;
	
	/* Drawable resources */
	private static SparseIntArray cardKindTinyLogoDrawables;
	
	/* Sounds */
	private static SparseIntArray sounds;
	private static SoundPool soundPool;
	
	/* Card templates */
	private static SparseIntArray cardTemplates;
	
	/* OpenGL resources */
	private static SparseArray<GLVertexBufferObject> vbos;
	private static SparseIntArray textures;
	private static SparseIntArray programs;
	private static SparseArray<PlanetFX> planetFXs;
	
	static {
		
		cardTinyImages = new SparseArray<Bitmap>();
		
		/* Cards little images */
			cardTinyImagesResID = new SparseIntArray();
			cardTinyImagesResID.put(Constants.IDC_HUNTER, R.drawable.card_hunter_tiny);
			cardTinyImagesResID.put(Constants.IDC_SCOUT, R.drawable.ship);
			cardTinyImagesResID.put(Constants.IDC_GHOST, R.drawable.ship);
			cardTinyImagesResID.put(Constants.IDC_MECHA_SQUAD, R.drawable.ship);
			cardTinyImagesResID.put(Constants.IDC_RAIDER, R.drawable.card_raider_tiny);
			cardTinyImagesResID.put(Constants.IDC_AEGIS, R.drawable.card_hunter_tiny);
		
		/* Card templates */
		cardTemplates = new SparseIntArray();

		/* Common */
			cardTemplates.put(Constants.IDC_LUCK_OF_THE_STARS, R.array.card_luckofthestars);
			cardTemplates.put(Constants.IDC_ORBITAL_DEFENCE, R.array.card_orbitaldefence);
			cardTemplates.put(Constants.IDC_PLANETARY_EXPLOITATION, R.array.card_planetaryexploitation);
		
		/* Heirs */
			cardTemplates.put(Constants.IDC_HUNTER, R.array.card_hunter);
			cardTemplates.put(Constants.IDC_SCOUT, R.array.card_scout);
			cardTemplates.put(Constants.IDC_RAIDER, R.array.card_raider);
			cardTemplates.put(Constants.IDC_GHOST, R.array.card_ghost);
			cardTemplates.put(Constants.IDC_MECHA_SQUAD, R.array.card_mechasquad);
			cardTemplates.put(Constants.IDC_AEGIS, R.array.card_aegis);
			

		
		/* Races tiny logos drawables */
		cardKindTinyLogoDrawables = new SparseIntArray();
		
		cardKindTinyLogoDrawables.put(Constants.CK_HEIRS, R.drawable.logo_heirs_tiny);
		cardKindTinyLogoDrawables.put(Constants.CK_COMMON, R.drawable.logo_common_tiny);
		
	}
	
	public static void initialize(Context ctx) {
		context = ctx;
		resources = ctx.getResources();
		displayDensity = resources.getDisplayMetrics().density;
		
		loadSoundPool();
		
	}
	
	public static float getDisplayDensity() {
		return displayDensity;
	}
	
	public static void loadCardTinyImages() {	
		
		for(int i = 0; i < cardTinyImagesResID.size(); i++) {
			int key = cardTinyImagesResID.keyAt(i);
			cardTinyImages.put(key, BitmapFactory.decodeResource(resources, cardTinyImagesResID.get(key)));
		}
	}
	
	public static int getCardKindTinyLogoDrawable(int kindID) { return cardKindTinyLogoDrawables.get(kindID); }
	
	public static Bitmap getCardKindTinyLogoMasked(int kindID, int overlayColor) {
		return MaskedBitmapFactory.decodeResource(resources, cardKindTinyLogoDrawables.get(kindID), overlayColor);
	}
	
	public static Bitmap getCardTinyImage(int cardID) {return cardTinyImages.get(cardID);}
	
	public static CardTemplate getCardTemplate(int cardID) {
		TypedArray array = resources.obtainTypedArray(cardTemplates.get(cardID));
		CardTemplate result =  new CardTemplate(array);
		array.recycle();
		return result;
	}
	
	/* Sounds */
	public static SoundPool getSoundPool() { return soundPool; }
	public static int getSoundID(int sound) { return sounds.get(sound); }
	
	/* OpenGL resources */
	public static PlanetFX getPlanetFX(int id) { return planetFXs.get(id); }
	public static GLVertexBufferObject getCommmonVBO(int id) {return vbos.get(id);}
	public static int getTexture(int id) {return textures.get(id);}
	public static int getProgram(int id) { return programs.get(id); }
	
	
	public static void loadPrograms() {
		programs = new SparseIntArray();
		
		programs.put(Constants.PRG_PLANET, GLU.loadProgram(R.string.planet_vs, R.string.planet_fs, resources));
		programs.put(Constants.PRG_PLANET_AURA, GLU.loadProgram(R.string.planet_aura_vs, R.string.planet_aura_fs, resources));
		programs.put(Constants.PRG_UNITS_ON_PLANET, GLU.loadProgram(R.string.unit_vs, R.string.unit_fs, resources));
		programs.put(Constants.PRG_ROUTES, GLU.loadProgram(R.string.routes_vs, R.string.routes_fs, resources));

		programs.put(Constants.PRG_ANIM_UNIT_MOVE, GLU.loadProgram(R.string.anim_unit_move_vs, R.string.anim_unit_move_fs, resources));		
		programs.put(Constants.PRG_ANIM_EXPLOSION, GLU.loadProgram(R.string.anim_explosion_vs, R.string.anim_explosion_fs, resources));
		
		programs.put(Constants.PRG_FX_PLANET_CAGE, GLU.loadProgram(R.string.effect_planet_cage_vs, R.string.effect_planet_cage_fs, resources));
		programs.put(Constants.PRG_FX_RESOURCE_LOCK, GLU.loadProgram(R.string.effect_resource_lock_vs, R.string.effect_resource_lock_fs, resources));

	}
	public static void loadTextures() {
		textures = new SparseIntArray();
		
		textures.put(Constants.TEX_PLANET_CAGE, GLU.loadTexture2D(R.drawable.tex_planet_cage, resources));
		textures.put(Constants.TEX_PLANET_AURA, GLU.loadTexture2D(R.drawable.tex_planet_aura, resources));
		textures.put(Constants.TEX_UNITS_ON_PLANET, GLU.loadTexture2D(R.drawable.tex_units_on_planet, resources));
		textures.put(Constants.TEX_NEBULA, GLU.loadTexture2D(R.drawable.tex_nebula, resources));
		textures.put(Constants.TEX_STAR, GLU.loadTexture2D(R.drawable.star, resources));
		
		textures.put(Constants.TEX_FX_RESOURCE_LOCK_0, GLU.loadTexture2D(R.drawable.tex_resource_lock_lg, resources));
		textures.put(Constants.TEX_FX_RESOURCE_LOCK_1, GLU.loadTexture2D(R.drawable.tex_resource_lock_hg, resources));
		
	}
	public static void loadCommonVBOs() {
		vbos = new SparseArray<GLVertexBufferObject>();
		GLVertexBufferObject vbo;
		Android3DObject obj;
		
		
		try {
			/* Load sphere */
			obj = Android3DObject.Load(resources.openRawResource(R.raw.a3o_sphere));
			vbo = obj.getShadingGroups().get(0).asVertexBuffer(GLES20.GL_STATIC_DRAW);
			vbo.setVertexCount(obj.getShadingGroups().get(0).getVertexCount());
			vbos.put(Constants.VBO_SPHERE, vbo);
			
			/* Load planet aura plane */
			obj = Android3DObject.Load(resources.openRawResource(R.raw.a3o_planetauraplane));
			vbo = obj.getShadingGroups().get(0).asVertexBuffer(GLES20.GL_STATIC_DRAW);
			vbo.setVertexCount(obj.getShadingGroups().get(0).getVertexCount());
			vbos.put(Constants.VBO_PLANET_AURA_PLANE, vbo);
			
			/* Load plane */
			obj = Android3DObject.Load(resources.openRawResource(R.raw.a3o_plane));
			vbo = obj.getShadingGroups().get(0).asVertexBuffer(GLES20.GL_STATIC_DRAW);	
			vbo.setVertexCount(obj.getShadingGroups().get(0).getVertexCount());
			vbos.put(Constants.VBO_PLANE, vbo);
			
			/* Missile */
			obj = Android3DObject.Load(resources.openRawResource(R.raw.a3o_missile));
			
			Log.d("Test3", obj.getShadingGroupCount() + "");
			
			for(Android3DObject.ShadingGroup s : obj.getShadingGroups())
				Log.d("Test3", s.getID());
		}
		catch(IOException ex) {
			Log.e(LOG_TAG, "Error while loading VBOs");
		}
	}
	public static void loadPlanetFXs() {
		planetFXs = new SparseArray<PlanetFX>();
		
		// TODO colori da mettere su costanti
		
		planetFXs.put(Constants.IDB_PLANETARY_CAGE, new PlanetaryCageFX());
		planetFXs.put(Constants.IDB_RESOURCE_DEPLETION, new ResourceLockFX(0xff00ff00));
		planetFXs.put(Constants.IDB_TACTICAL_BOMBING, new NoFX());
		planetFXs.put(Constants.IDB_ORBITAL_DEFENCE, new NoFX());
		planetFXs.put(Constants.IDB_FLYING_AEGIS, new NoFX());
		planetFXs.put(Constants.IDB_THERMONUCLEAR_BOMBING, new ResourceLockFX(0xffff0000));
	}	
	
	private static void loadSoundPool() {
		sounds = new SparseIntArray();
		soundPool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
		
		sounds.put(Constants.SND_DISTANT_EXPLOSION, soundPool.load(context, R.raw.mp3_dexplosion, 1));
	}
}
