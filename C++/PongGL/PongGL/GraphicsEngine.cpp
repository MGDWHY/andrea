#include "GraphicsEngine.h"

#define PI 3.141592

using namespace glutils;

const int GraphicsEngine::NUM_PP_FILTERS = 6;

const int GraphicsEngine::PP_NOFILTER = 0;
const int GraphicsEngine::PP_DEPTH = 1;
const int GraphicsEngine::PP_BLOOM = 2;
const int GraphicsEngine::PP_OLDTV = 3;
const int GraphicsEngine::PP_WAVE = 4;
const int GraphicsEngine::PP_POOR = 5;

const int GraphicsEngine::NUM_CAMERA_MODES = 2;

const int GraphicsEngine::CAMERA_FREE = 0;
const int GraphicsEngine::CAMERA_SHIP = 1;

const string GraphicsEngine::PP_FILTER_NAMES[] = { "NONE", "DEPTH OF FIELD", "BLOOM", "OLD TV", "WAVE", "125 COLORS" };

GraphicsEngine::GraphicsEngine(GameEngine *engine)
{
	this->engine = engine;
	
	// palms dispalcement
	palmsDisplacement[0] = vec3(-10, -3, -10);
	palmsDisplacement[1] = vec3(-10, -3, 10);
	palmsDisplacement[2] = vec3(10, -3, 0);
	palmsDisplacement[3] = vec3(15, -3, -15);

	// palms orientation
	palmsRotY[0] = 0;
	palmsRotY[1] = 90;
	palmsRotY[2] = 180;
	palmsRotY[3] = -45;
	
	initShaders();
	initDisplayLists();
	initTextures();
	initMaterials();
	initLights();
	initFrameBuffer();
	initSplines();

	postprocessingFilter = PP_NOFILTER;
	cameraMode = CAMERA_FREE;

	wireframe = false;
}

GraphicsEngine::~GraphicsEngine(void)
{
}

void GraphicsEngine::pngLoad(const char* filename) {
 
  unsigned int width, height;
  std::vector<unsigned char> *image = new std::vector<unsigned char>;
  unsigned error = LodePNG::decode(*image, width, height, filename);
  unsigned char* data = new unsigned char[width*height*4];

  for(unsigned int i = 0; i < width * height * 4; i++)
	  data[i] = (*image)[i];


  gluBuild2DMipmaps ( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  delete image;
  delete data;
}
GLuint GraphicsEngine::pngTexture(const char* filename) {
	GLuint texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	pngLoad(filename);

	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;
		msgss << "Texture loaded: " << filename;
		Logger::getDefaultLogger()->writeMessageStream(0, "GraphicsEngine::pngTexture()", msgss);
	#endif

	return texid;
}

void GraphicsEngine::drawBox(GLfloat width, GLfloat height, GLfloat length, GLfloat tx, GLfloat ty, GLfloat tz) {
	glBegin(GL_QUADS);
		// FRONT FACE
		glNormal3f(0,0,1);	
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, length/2);
		glTexCoord2f(tx, ty);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, length/2);

		// BACK FACE
		glNormal3f(0,0,-1);	
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(tx, ty);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, -length/2);

		// LEFT FACE
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, ty);	glVertex3f(-width/2, -height/2, -length/2);
		glTexCoord2f(tz, ty);	glVertex3f(-width/2, -height/2, length/2);
		glTexCoord2f(tz, 0);	glVertex3f(-width/2, height/2, length/2);

		// RIGHT FACE
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0);		glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(tz, 0);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(tz, ty);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(0, ty);	glVertex3f(width/2, -height/2, -length/2);

		//TOP FACE
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, tz);	glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(tx, tz);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, height/2, -length/2);

		//BOTTOM FACE
		glNormal3f(0, -1, 0);
		glTexCoord2f(0, 0);		glVertex3f(-width/2, -height/2, -length/2);
		glTexCoord2f(tx, 0);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(tx, tz);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(0, tz);	glVertex3f(-width/2, -height/2, length/2);
	glEnd();
}
void GraphicsEngine::drawString(string s) {

	glPushMatrix();

	for(unsigned int i = 0; i < s.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);

	glPopMatrix();

}

void GraphicsEngine::initShaders() {
	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;

		msgss	<< "------------------------------" << endl
				<< "\tLOADING SHADERS" <<  endl
				<< "------------------------------";

		Logger::getDefaultLogger()->writeMessageStream(0, "", msgss);

	#endif
	std::string path = "D:\\Andrea\\GLSL\\";

	//Loading shaders
	shaderManager = ShaderManager::getDefaultManager();
	pplighting = shaderManager->createShader(path + "pplighting\\pplighting.vert", path + "pplighting\\pplighting.frag");
	bumpMapping = shaderManager->createShader(path + "bumpmapping\\bump1.vert", path + "bumpmapping\\bump1.frag");
	water = shaderManager->createShader(path + "water\\water4.vert", path + "water\\water4.frag");
	bloom = shaderManager->createShader(path + "postprocessing\\bloom.vert", path + "postprocessing\\bloom.frag");
	oldTV = shaderManager->createShader(path + "postprocessing\\blackAndWhite.vert", path + "postprocessing\\oldtv.frag");
	wave = shaderManager->createShader(path + "postprocessing\\wave.vert", path + "postprocessing\\wave.frag");
	poor = shaderManager->createShader(path + "postprocessing\\poorcolor.vert", path + "postprocessing\\poorcolor.frag");
	depth = shaderManager->createShader(path + "postprocessing\\depth.vert", path + "postprocessing\\depth.frag");
}
void GraphicsEngine::initSplines() {

	vec3 cps1[] = {
		vec3(-10, 5, 10),
		vec3(0, 5, 0),
		vec3(-5, 6, 0),
		vec3(-5, 9, 5),
		vec3(10, 8, 10),
		vec3(10, 6, -6),
		vec3(-10, 8, -12),
		vec3(-10, 5, 10),
		vec3(0, 5, 0)
	};
	float knots1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	splineLight1 = new Bspline(3, 9, cps1, knots1);

	vec3 cps2[] = {
		vec3(-20, 14, -20),
		vec3(-20, 13, 20),
		vec3(0, 6, 0),
		vec3(20, 3, -20),
		vec3(20, 10, 20),
		vec3(0, 5, 0),
		vec3(-20, 14, -20),
		vec3(-20, 13, 20)
	};

	float knots2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	splineLight2 = new Bspline(3, 8, cps2, knots2);

	vec3 cps3[] = {
		vec3(-20, -1.5, -20),
		vec3(15, -1.5, -20),
		vec3(20, -1.5, 20),
		vec3(-20, -1.5, 20),
		vec3(-20, -1.5, -20),
		vec3(15, -1.5, -20) 
	};
	float knots3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	sailShip = new Bspline(3, 6, cps3, knots3);
}
void GraphicsEngine::initFrameBuffer() {

	fbo = new FrameBufferObject(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	
	reflectionsColorTarget = fbo->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	screenColorTarget = fbo->createAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	screenDepthTarget = fbo->createAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
	
	glPushAttrib(GL_TEXTURE_BIT);

	// texture rendering targets must be clamped
	
	glBindTexture(GL_TEXTURE_2D, screenColorTarget);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, reflectionsColorTarget);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPopAttrib(); // texture bit

	fbo->attach(GL_DEPTH_ATTACHMENT, screenDepthTarget, GL_TEXTURE_2D);
}
void GraphicsEngine::initDisplayLists() {
	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;
		msgss	<< "-----------------------------------------------" << endl
				<< "\tLOADING MODELS & DISPLAY LISTS" <<  endl
				<< "-----------------------------------------------";

		Logger::getDefaultLogger()->writeMessageStream(0, "", msgss);
	#endif

	//water
	float step = 10.0f / 100.0f;
	waterList = glGenLists(1);
	glNewList(waterList, GL_COMPILE);
		glNormal3f(0,1,0);
		glBegin(GL_QUADS);
			for(int x = -50; x < 50; x++)
				for(int z = -50; z < 50; z++) {
					
					glTexCoord2f(x*step, z*step);
					glVertex3f(x, 0, z);

					glTexCoord2f(x*step, (z+1)*step);
					glVertex3f(x, 0, z + 1);

					glTexCoord2f((x+1)*step, (z+1)*step);
					glVertex3f(x + 1, 0, z + 1);
					
					glTexCoord2f((x+1)*step, z*step);
					glVertex3f(x +1, 0, z);
				}
		glEnd();
	glEndList();

	fieldList = glGenLists(1);

	//field
	glNewList(fieldList, GL_COMPILE);
		glPushMatrix();
		glTranslatef(0, -0.1f, 0.0);
		drawBox(engine->getFieldWidth(), 0.2f, engine->getFieldLength(),
			engine->getFieldWidth()/3, 0.2f/3, engine->getFieldLength()/3);
		glPopMatrix();
	glEndList();

	wallsList = glGenLists(1);
	glNewList(wallsList, GL_COMPILE);
		//left wall
		glPushMatrix();
		glTranslatef(-engine->getFieldWidth() / 2 - 0.1f, 0.3f, 0);
		drawBox(0.2f, 1.0f, engine->getFieldLength(), 0.2f/3.0f, 1.0f/3.0f, engine->getFieldLength()/3.0f);
		glPopMatrix();

		//rightWall
		glPushMatrix();
		glTranslatef(engine->getFieldWidth() / 2 + 0.1f, 0.3f, 0);
		drawBox(0.2f, 1.0f, engine->getFieldLength(), 0.2f/3.0f, 1.0f/3.0f, engine->getFieldLength()/3.0f);
		glPopMatrix();
	glEndList();

	padList = glGenLists(1);

	// pads
	glNewList(padList, GL_COMPILE);
		glPushMatrix();
		glTranslatef(0, 0.5, 0);
		drawBox(engine->getPadsWidth(), 1, 1, engine->getPadsWidth(), 1, 1);
		glPopMatrix();
	glEndList();

	//ball
	ballList = glGenLists(1);
	glNewList(ballList, GL_COMPILE);
		glutSolidSphere(0.5, 50, 50);
	glEndList();


	//pylons
	GLUquadric *quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricOrientation(quad, GLU_OUTSIDE);

	pylonList = glGenLists(1);
	glNewList(pylonList, GL_COMPILE);
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		gluCylinder(quad, 0.5, 0.5, 3, 25, 25);
		glPopMatrix();
	glEndList();

	//lights
	pointLightList = glGenLists(1);
	glNewList(pointLightList, GL_COMPILE);
		glutSolidSphere(0.1f, 16, 16);
	glEndList();

	// MODELS

	ObjectFile *obj;

	//ship
	obj = ObjectFile::load("models/ship_boat.obj");
	
	shipMaterials = MaterialLibrary::load("models/ship_boat.mtl");
	shipLists = new GLuint[shipMaterials->getMaterialCount()];
	for(int i = 0; i < shipMaterials->getMaterialCount(); i++) {
		string name = shipMaterials->getMaterial(i).getName();
		shipLists[i] = obj->createDisplayList(NULL, new string(name));
	}

	//palms
	obj = ObjectFile::load("models/palm.obj");
	palmList[0] = obj->createDisplayList(new string("palm_log"), NULL);
	palmList[1] = obj->createDisplayList(new string("palm_leaves"), NULL);

	delete obj;
}
void GraphicsEngine::initMaterials() {
	floor.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	floor.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	floor.shininess = 64;

	pads.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	pads.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pads.shininess = 64;

	ground.diffuse = vec4(0.8f, 0.8f, 0.2f, 1.0f);
	ground.shininess = 128;
}
void GraphicsEngine::initTextures() {
	#ifdef GL_UTILS_LOG_ENABLED
		stringstream msgss;

		msgss	<< "------------------------------" << endl
				<< "\tLOADING TEXTURES" <<  endl
				<< "------------------------------";

		Logger::getDefaultLogger()->writeMessageStream(0, "", msgss);

	#endif
	fieldTexture = pngTexture("textures/field.png");
	fieldBump = pngTexture("textures/fieldBump.png");
	waterTexture = pngTexture("textures/water.png");
	waterNormal1 = pngTexture("textures/waterNormal.png");
	waterNormal2 = pngTexture("textures/waterNormal2.png");
	noiseTexture = pngTexture("textures/noise.png");
	palmLeaves = pngTexture("textures/palm_texture.png");
	palmLog = pngTexture("textures/palm_texture2.png");
	

	shipTextures = new GLuint[shipMaterials->getMaterialCount()];
	for(int i = 0; i < shipMaterials->getMaterialCount(); i++) {
		Material m = shipMaterials->getMaterial(i);
		if(m.getMapKd().compare("") == 0) //no texture
			shipTextures[i] = 0;
		else {
			string texpath = "textures/" + m.getMapKd();
			shipTextures[i] = pngTexture(texpath.c_str());
		}
	}

	skyTextures = new GLuint[5];

	//skybox textures must be clamped
	skyTextures[0] = pngTexture("textures/sky/nightsky_lf.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skyTextures[1] = pngTexture("textures/sky/nightsky_rt.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skyTextures[2] = pngTexture("textures/sky/nightsky_ft.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skyTextures[3] = pngTexture("textures/sky/nightsky_bk.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skyTextures[4] = pngTexture("textures/sky/nightsky_up.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void GraphicsEngine::initLights() {
	//light1
	light1.diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1.ambient.rgba);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.002f);
	glEnable(GL_LIGHT1);

	//light2
	light2.diffuse = vec4(0.0f, 1.0f, 0.0f, 1.0f);

	glLightfv(GL_LIGHT2, GL_AMBIENT, light2.ambient.rgba);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.002f);
	glEnable(GL_LIGHT2);

	//ball light
	light3.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light3.ambient = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	light3.position = vec4(0.0f, 2.0f, 0.0f, 1.0f);

	glLightfv(GL_LIGHT3, GL_AMBIENT, light3.ambient.rgba);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3.diffuse.rgba);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3.diffuse.rgba);
	glLightfv(GL_LIGHT3, GL_POSITION, light3.position.xyzw);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0f); 
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.01f);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.05f);
	glEnable(GL_LIGHT3);

}

void GraphicsEngine::render(float cameraRotX, float cameraRotY, float cameraZoom, int viewWidth, int viewHeight) {
	static GLdouble waterClip[] = { 0.0f, -1.0f, 0.0f, -1.0f };
	static Timer *timer = new Timer();
	static vec3 cameraPos; // camera position wcs
	static float dt, time;

	time = timer->timeSecs(); // total time from glutInit call
	dt = timer->dtSecs(); // time passed from the last call of dtSecs()

	update(dt, time); // update moving objects

	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)viewWidth/viewHeight,0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//-------------------------CAMERA----------------------------
	if(cameraMode == CAMERA_FREE) { // free camera, static sky
		glTranslatef(0,0, cameraZoom);
		glRotatef(cameraRotX, 1, 0, 0);
		glRotatef(cameraRotY, 0, 1, 0);
		cameraPos = vec3();
	} else if(cameraMode == CAMERA_SHIP) { // sailship camera, dynamic sky
		static vec3 dir, eye, center;
		
		dir = normalize(sub(shipPoint2, shipPoint1));
		cameraPos = eye = add(shipPoint1, vec3(0.0, 4.0, 0));
		center = add(eye, dir);

		glRotatef(cameraRotX, 1, 0, 0);
		glRotatef(cameraRotY, 0, 1, 0);
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			0, 1, 0
		);
	}

	//-------------------------WATER REFLECTIONS-----------------------
	// renderering reflections to a texture
	fbo->attach(GL_COLOR_ATTACHMENT0, reflectionsColorTarget, GL_TEXTURE_2D);
	fbo->enable();

	glViewport(0,0,fbo->getWidth(), fbo->getHeight());

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// enable clipping on pplighting shader
	pplighting->enable();
	pplighting->setUniform("clip_0123", true, false, false, false);
	pplighting->disable();

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
	glCullFace(GL_FRONT); // back faces become front faces while reflecting
	glEnable(GL_CLIP_PLANE0);
	
	glClipPlane(GL_CLIP_PLANE0, waterClip); // clip the pixels above water plane ( y = -1)

	glPushMatrix();
	glTranslatef(0,-2,0);
	glScalef(1,-1,1); // reflect by the xz plane
	renderSky(cameraPos); // render skybox
	renderScene(dt, time); // render the scene
	glPopMatrix();
	
	glPopAttrib(); // enable bit, polygon bit

	// disable clipping on pplighting shader
	pplighting->enable();
	pplighting->setUniform("clip_0123", false, false, false, false);
	pplighting->disable();

	fbo->disable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionsColorTarget);
	glGenerateMipmap(GL_TEXTURE_2D);

	//--------------------------SCENE----------------------------
	if(postprocessingFilter != 0) { // postprocessing enabled, render to fbo
		fbo->attach(GL_COLOR_ATTACHMENT0, screenColorTarget, GL_TEXTURE_2D);
		fbo->enable();
		glViewport(0,0,fbo->getWidth(), fbo->getHeight());
	} else // postprocessing disabled, render to screen
		glViewport(0,0,viewWidth, viewHeight);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	renderSky(cameraPos); // render skybox
	renderScene(dt, time); // render the scene
	renderWater(time); // render water surface

	if(postprocessingFilter != 0) 
		fbo->disable(); // disable fbo


	//-------------------------POST PROCESSING--------------------------
	// rendering the offscreen buffer to a textured fullviewport quad

	if(postprocessingFilter != 0) {

		glViewport(0,0,viewWidth, viewHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewWidth, 0, viewHeight, 0.1, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0,0,-0.5);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenColorTarget);
		glGenerateMipmap(GL_TEXTURE_2D);

		if(postprocessingFilter == PP_BLOOM) {
			bloom->enable();
			bloom->setUniformTexture("texture", 0, screenColorTarget);
			bloom->setUniform("stepS", 0.0025f);
			bloom->setUniform("stepT", 0.0025f);
		} else if(postprocessingFilter == PP_OLDTV) {
			oldTV->enable();
			oldTV->setUniform("rand", Randomizer::nextFloat());
			oldTV->setUniform("scale", 0.05f);
			oldTV->setUniformTexture("texture", 0, screenColorTarget);
			oldTV->setUniformTexture("noise", 1, noiseTexture);
		} else if(postprocessingFilter == PP_WAVE) {
			wave->enable();
			wave->setUniform("time", time);
			wave->setUniformTexture("texture", 0, screenColorTarget);
		} else if(postprocessingFilter == PP_POOR) {
			poor->enable();
			poor->setUniformTexture("texture", 0, screenColorTarget);
		} else if(postprocessingFilter == PP_DEPTH) {
			depth->enable();
			depth->setUniformTexture("texture", 0, screenColorTarget);
			depth->setUniformTexture("zbuffer", 1, screenDepthTarget);
			depth->setUniform("stepS", 0.001f);
			depth->setUniform("stepT", 0.0025f);
		}

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(1, 0); glVertex2f(viewWidth, 0);
			glTexCoord2f(1, 1); glVertex2f(viewWidth, viewHeight);
			glTexCoord2f(0, 1); glVertex2f(0, viewHeight);
		glEnd();

		bloom->disable();

		glPopAttrib();
	}

	//------------------------------INFO---------------------------------
	// rendering scene info

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewWidth, 0, viewHeight, 0.1, 1.0);

	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(20, viewHeight - 20, 0.0f);
	renderInfo(dt, time);

	glPopAttrib(); // enable bit
}
void GraphicsEngine::renderScene(float dt, float time) {

	//light1
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos.xyzw);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1.diffuse.rgba);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1.diffuse.rgba);

	//light2
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos.xyzw);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2.diffuse.rgba);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2.diffuse.rgba);

	//light ball
	light3.spotDirection.x = engine->getBallX();
	light3.spotDirection.y = -light3.position.y;
	light3.spotDirection.z = engine->getBallY();
	glLightfv(GL_LIGHT3, GL_POSITION, light3.position.xyzw);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3.spotDirection.xyz);

	//enable per-pixel lighting shader
	pplighting->enable();

	// render sail ship
	glPushMatrix();
	renderSailShip();
	glPopMatrix();

	pplighting->enable();
	pplighting->setUniform("useTexture", false);
	pplighting->setUniform("lgt_0123", false, true, true, true);
	pplighting->setUniform("reflectivePower", 0.2f);

	
	// meterial for pads, ball and pylons
	glMaterialf(GL_FRONT, GL_SHININESS, pads.shininess);

	// draw pad 1 (front pad)
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(engine->getPad1X(), 0, engine->getFieldLength() / 2 + 0.5);
	glCallList(padList);
	glPopMatrix();

	//draw pad 2 (back pad)
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(engine->getPad2X(), 0, - engine->getFieldLength() / 2 - 0.5);
	glCallList(padList);
	glPopMatrix();

	pplighting->setUniform("reflectivePower", 1.0f);

	// draw ball
	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.8f);
	glTranslatef(engine->getBallX(), 0.5, engine->getBallY());
	glCallList(ballList);
	glPopMatrix();

	//draw pylons
	glColor3f(0.5f, 0.5f, 1.0f);

	glPushMatrix();
	glTranslatef(-engine->getFieldWidth()/2 + 0.5f, -0.2, engine->getFieldLength()/2 -0.5f);
	glCallList(pylonList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(engine->getFieldWidth()/2 - 0.5f, -0.2, engine->getFieldLength()/2 -0.5f);
	glCallList(pylonList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(engine->getFieldWidth()/2 - 0.5f, -0.2, -engine->getFieldLength()/2 +0.5f);
	glCallList(pylonList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-engine->getFieldWidth()/2 + 0.5f, -0.2, -engine->getFieldLength()/2 +0.5f);
	glCallList(pylonList);
	glPopMatrix();

	pplighting->setUniform("useTexture", false);

	//render walls
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 1.0f);
	glMaterialf(GL_FRONT, GL_SHININESS, floor.shininess);
	glCallList(wallsList);
	glPopMatrix();

	bumpMapping->enable();
	bumpMapping->setUniform("useTexture", true);
	bumpMapping->setUniform("lgt_0123", false, true, true, true);
	bumpMapping->setUniform("tangent", vec3(1.0f, 0.0f, 0.0f));
	bumpMapping->setUniform("reflectivePower", 0.5f);
	bumpMapping->setUniformTexture("texture", 0, fieldTexture);
	bumpMapping->setUniformTexture("normalMap", 1, fieldBump);

	//render field
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glMaterialf(GL_FRONT, GL_SHININESS, floor.shininess);
	glCallList(fieldList);
	glPopMatrix();

	//palms
	renderPalms();

	glPushAttrib(GL_ENABLE_BIT);

	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// draw light1
	glPushMatrix();

	glColor4fv(light1.diffuse.rgba);
	glTranslatef(light1Pos.x, light1Pos.y, light1Pos.z);
	glCallList(pointLightList);
	glPopMatrix();

	// draw light2
	glPushMatrix();
	glColor4fv(light2.diffuse.rgba);
	glTranslatef(light2Pos.x, light2Pos.y, light2Pos.z);
	glCallList(pointLightList);
	glPopMatrix();

	glPopAttrib();
}
void GraphicsEngine::renderInfo(float dt, float time) {
	static int fps = 0;
	static float t = 0;
	static string str = "0";

	if(t >= 1.0f) {
		stringstream ss;
		ss << fps << " fps";
		getline(ss, str);
		
		t = 0.0f;
		fps = 0;
	}
	
	glPushAttrib(GL_ENABLE_BIT);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.5f);
	glScalef(0.1f, 0.1f, 0.1f);
	drawString(str);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -20.0f, -0.5f);
	glScalef(0.1f, 0.1f, 0.1f);
	drawString("Active Filter: " + PP_FILTER_NAMES[postprocessingFilter]);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, -40.0f, -0.5f);
	glScalef(0.1f, 0.1f, 0.1f);
	
	if(wireframe)
		drawString("Polygon Mode: WIREFRAME");
	else
		drawString("Polygon Mode: FILL");

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -60.0f, -0.5f);
	glScalef(0.1f, 0.1f, 0.1f);
	
	if(cameraMode == CAMERA_FREE)
		drawString("Camera: FREE");
	else
		drawString("Camera: SHIP");

	glPopMatrix();

	glPopAttrib(); // enable bit

	t += dt;
	fps++;
}
void GraphicsEngine::renderPalms() {

	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE); 

	pplighting->enable();
	pplighting->setUniform("reflectivePower", 0.0f);

	for(int i = 0; i < 4; i++) {
		
		pplighting->setUniform("useTexture", true);
	
		glPushMatrix();
	
		glTranslatef(palmsDisplacement[i].x, palmsDisplacement[i].y, palmsDisplacement[i].z);
		glRotatef(palmsRotY[i], 0, 1, 0);
		
		pplighting->setUniformTexture("texture", 0, palmLeaves);
		glColor3f(0, 1, 0);
		glCallList(palmList[1]); // leaves

		pplighting->setUniformTexture("texture", 0, palmLog);
		glColor3f(1, 0, 1);
		glCallList(palmList[0]); // log

		glPopMatrix();
	}

	glPopAttrib(); // enable bit

	pplighting->disable();
}
void GraphicsEngine::renderSailShip() {
	static GLfloat matrix[16]; // rotation matrix
	static vec3 posX, posZ, posY;

	// Ship space's axises
	posX = normalize(sub(shipPoint2, shipPoint1));
	posY = vec3(0, 1, 0);
	posZ = normalize(cross(posX, posY));
	
	// Ship rotation matrix
	/*	
	------------------------------
		dirX.x	dirX.y	dirX.z	0
		dirY.x	dirY.y	dirY.z	0
		dirZ.x	dirZ.y	dirZ.z	0
		0		0		0		1
	------------------------------
	*/

	//column 1
	matrix[0] = posX.x;
	matrix[1] = posY.x;
	matrix[2] = posZ.x;
	matrix[3] = 0;

	//column 2
	matrix[4] = posX.y;
	matrix[5] = posY.y;
	matrix[6] = posZ.y;
	matrix[7] = 0;

	//column 3
	matrix[8] = posX.z;
	matrix[9] = posY.z;
	matrix[10] = posZ.z;
	matrix[11] = 0;

	//column 4
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	glPushMatrix();

	// M = Mtranslate * transpose(Mrotate) * Mscale
	glTranslatef(shipPoint1.x, shipPoint1.y, shipPoint1.z); // Mtranslate

	glMultTransposeMatrixf(matrix); //Mrotate
	glRotatef(110, 0, 1, 0); // rotate to align the ship with the x-ocs axis

	glScalef(0.002, 0.002, 0.002); // Mscale

	pplighting->setUniform("useTexture", false);
	for(int i = 0; i < shipMaterials->getMaterialCount(); i++) {
		
		Material m = shipMaterials->getMaterial(i);

		pplighting->setUniform("reflectivePower", m.getKs().r);

		if(shipTextures[i] == 0) { // no texture... use color
			pplighting->setUniform("useTexture", false);
			glColor3fv(m.getKd().rgb);
		} else { // use texture as diffuse color
			pplighting->setUniform("useTexture", true);
			pplighting->setUniformTexture("texture", 0, shipTextures[i]);
		}

		glCallList(shipLists[i]);
	}
	glPopMatrix();
}
void GraphicsEngine::renderWater(float time) {
	// enable water shader
	water->enable();
	water->setUniform("useTexture", true);
	water->setUniform("reflectivePower", 0.2f);
	water->setUniform("time", time);
	water->setUniform("lgt_0123", false, true, true, true);
	water->setUniformTexture("normalMap1", 0, waterNormal1);
	water->setUniformTexture("normalMap2", 1, waterNormal2);
	water->setUniformTexture("reflections", 2, reflectionsColorTarget);

	//glPushAttrib(GL_ENABLE_BIT);
	//glDisable(GL_CULL_FACE);
	// render water
	glPushMatrix();
	glColor4f(.7f, .8f, 1.0f, .2f);
	glTranslatef(0.0, -1.0, 0.0);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
	glCallList(waterList);
	glPopMatrix();

	//glPopAttrib();

	water->disable();
}
void GraphicsEngine::renderSky(vec3 cameraPos) {
	static GLfloat width = 100, height = 100, length = 100;
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	

	glPushMatrix();
	
	glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);

	glColor3f(1.0f, 1.0f, 1.0f);

	// FRONT FACE
	glBindTexture(GL_TEXTURE_2D, skyTextures[2]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1, 0);	glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(0, 0);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(0, 1);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(1, 1);	glVertex3f(-width/2, -height/2, length/2);
	glEnd();

	// BACK FACE
	glBindTexture(GL_TEXTURE_2D, skyTextures[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, 1);	glVertex3f(-width/2, -height/2, -length/2);
		glTexCoord2f(1, 1);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(1, 0);	glVertex3f(width/2, height/2, -length/2);
	glEnd();

	// LEFT FACE
	glBindTexture(GL_TEXTURE_2D, skyTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);	glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, 0);	glVertex3f(-width/2, height/2, length/2);
		glTexCoord2f(0, 1);	glVertex3f(-width/2, -height/2, length/2);
		glTexCoord2f(1, 1);	glVertex3f(-width/2, -height/2, -length/2);
	glEnd();
		
	// RIGHT FACE
	glBindTexture(GL_TEXTURE_2D, skyTextures[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(0, 1);	glVertex3f(width/2, -height/2, -length/2);
		glTexCoord2f(1, 1);	glVertex3f(width/2, -height/2, length/2);
		glTexCoord2f(1, 0);	glVertex3f(width/2, height/2, length/2);
	glEnd();
		
	//TOP FACE
	glBindTexture(GL_TEXTURE_2D, skyTextures[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);	glVertex3f(-width/2, height/2, -length/2);
		glTexCoord2f(0, 0);	glVertex3f(width/2, height/2, -length/2);
		glTexCoord2f(0, 1);	glVertex3f(width/2, height/2, length/2);
		glTexCoord2f(1, 1);	glVertex3f(-width/2, height/2, length/2);
	glEnd();

	glPopMatrix();

	glPopAttrib(); // texture bit, enable bit
}

void GraphicsEngine::update(float dt, float time) {
	static float delta = 0.05;
	static float tship = 0.0f;
	static float t1 = 0, t2 = 0, tColor;

	t1 += dt;
	t2 += dt;
	
	tColor = cos(time) / 2.0f;

	if(t1 > 7.0f)
		t1 = 0.0f;

	if(t2 > 6.0f)
		t2 = 0.0f;

	// lights position
	light1Pos = vec4(splineLight1->evaluate(t1 + 2.0f), 1.0f);
	light2Pos = vec4(splineLight2->evaluate(t2 + 2.0f), 1.0f);

	// lights color
	light1.diffuse.b = 0.5f + tColor;
	light2.diffuse.b = 0.5f + tColor;
	

	tship += dt/8.0;

	if(tship > 4.0f)
		tship = 0.0f;

	// ship position
	shipPoint1 = sailShip->evaluate(2.0f + tship);
	shipPoint2 = sailShip->evaluate(2.0f + tship + delta);
}

void GraphicsEngine::toggleFilters() { postprocessingFilter = (postprocessingFilter + 1) % NUM_PP_FILTERS;}
void GraphicsEngine::toggleWireframe() { wireframe = !wireframe; }
void GraphicsEngine::toggleCamera() { cameraMode = (cameraMode + 1) % NUM_CAMERA_MODES;}