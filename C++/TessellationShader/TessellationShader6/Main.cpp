#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#include <Windows.h>
#include <gl\glew.h>
#include <gl\glut.h>
#include <ShaderManager.h>
#include <MatrixStack.h>
#include <wfo.h>
#include <VertexArrayObject.h>
#include <vecmath.h>
#include <lodepng.h>
#include <iostream>

#define GL_ERROR_CHECK { int x = glGetError(); if(x != GL_NO_ERROR) std::cout << "GL Error: " << x << std::endl; }
#define PI 3.141592f
#define MESH "pawn.obj"

struct Vertex {
	vec3 Position;
	vec3 Normal;
};

/* Descrittore di un patch di Bezier */

struct Bezier4x4Desc {
	vec3 ControlPoints[16];		// Punti di controllo
	vec3 OriginalQuad[4];		// Quad originale
	vec3 Normals[4];			// Normali
	Bezier4x4Desc* adjLeft;		// Adiacenza a sinistra
	Bezier4x4Desc* adjRight;	// Adiacenza a destra
	Bezier4x4Desc* adjTop;		// Adiacenza in alto
	Bezier4x4Desc* adjBottom;	// Adiacenza in basso

	Bezier4x4Desc Rotate() {	// Routa la patch in senso antiorario
		Bezier4x4Desc result;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				result.ControlPoints[i * 4 + j] = ControlPoints[j * 4 +  3 - i];

		result.OriginalQuad[0] = OriginalQuad[1];
		result.OriginalQuad[1] = OriginalQuad[2];
		result.OriginalQuad[2] = OriginalQuad[3];
		result.OriginalQuad[3] = OriginalQuad[0];

		return result;
		
	}

};

/* 
	Patch di Bezier. La GPU riceve in input un array di queste strutture.
	Dimensione: 4 * 3 * 16 = 192 bytes
*/
struct Bezier4x4 {
	Vertex ControlPoints[16];			// Punti di controllo

	Bezier4x4() {
		for(int i = 0; i <  16; i++)
			ControlPoints[i] = Vertex();
	}

	Bezier4x4(Bezier4x4Desc d) {	// Costruisce una patch a partire dal descrittore
		for(int i = 0; i <  16; i++)
			ControlPoints[i].Position = d.ControlPoints[i];

		ControlPoints[0].Normal = d.Normals[0];
		ControlPoints[3].Normal = d.Normals[1];
		ControlPoints[15].Normal = d.Normals[2];
		ControlPoints[12].Normal = d.Normals[3];
	}
};



using namespace Vecmath;
using namespace WaveFront;
using namespace glutils;


void InitGL();
void InitShaders();
void InitBuffers();
void InitTextures();

void ComputePatches(WFObject * obj);
void ComputeCornerPoints(int currentPatch, vector<Bezier4x4Desc>& patches);
void GetPatchesSharingCorner(int currentPatch, int corner, vector<Bezier4x4Desc>& patches, vector<Bezier4x4Desc>& neighbours);
void GetCornerNeighbours(Bezier4x4Desc& patch, int corner, vector<vec3>& neighbours);
int FindCorner(Bezier4x4Desc& patch, vec3 corner);

void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void MouseMotion(int, int);
void Mouse(int, int, int, int);
void PngLoad(const char* filename);
GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter);

MatrixStack *gStack;
Shader *gBezierShader, *gNormalShader, *gLineShader;
VertexArrayObject *gVAO;
vector<Bezier4x4> patches;

mat4 gModelView, gProjection;

float gWidth = 800, gHeight = 800;
float gTessLevel = 2.0f;
float rotX = 0.0f, rotY = 0.0f;
float prevX, prevY;
float zCam = -8;

bool dragging = false;

bool showPoints = false, interpolatedNormals = true, showNormals = false;

// Vector
template<class T> bool Contains(vector<T>& v, T& element) {
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == element)
			return true;
	}
	return false;
}

 int main(int argc, char **argv) {
	
	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize((int)gWidth, (int)gHeight);
	// create window
	glutCreateWindow("Tessellation Shader #3");
	// Finestra creata... Adesso dovrebbe andare, ma

	
	glewInit();
	
	InitGL();
	InitShaders();
	InitBuffers();
	InitTextures();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(MouseMotion);
	glutMouseFunc(Mouse);

	glutFullScreen();

	glutMainLoop();

}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
	gWidth = w;
	gHeight = h;

	gStack->loadIdentity();
	gStack->perspective(45, gWidth /gHeight, 0.1, 100);
	gProjection = gStack->current();
}

// called when window is drawn
void Render() {

	// model-view transform

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	gStack->loadIdentity();
	gStack->translate(0, 0, zCam);
	gStack->rotate(rotX, 1, 0, 0);
	gStack->rotate(rotY, 0, 1, 0);
	gModelView = gStack->current();

	
	if(showPoints) 
	{
		gLineShader->enable();
			gLineShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
			gLineShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
		
			glPointSize(5);
			glBindVertexArray(gVAO->getID());
			glDrawArrays(GL_POINTS, 0, patches.size() * 16);
			glBindVertexArray(0);

		gLineShader->disable();
	}

	
	if(showNormals) {
		gNormalShader->enable();

			gNormalShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
			gNormalShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
			gNormalShader->setUniform("in_TessLevel", gTessLevel);
			gNormalShader->setUniform("in_InterpolatedNormals", interpolatedNormals);

			glPatchParameteri(GL_PATCH_VERTICES, 16);

			glBindVertexArray(gVAO->getID());
			glDrawArrays(GL_PATCHES, 0, patches.size() * 16);
			glBindVertexArray(0);

		gNormalShader->disable();
	}
	
	gBezierShader->enable();

		gBezierShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
		gBezierShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
		gBezierShader->setUniform("in_TessLevel", gTessLevel);
		gBezierShader->setUniform("in_InterpolatedNormals", interpolatedNormals);

		glPatchParameteri(GL_PATCH_VERTICES, 16);

		glBindVertexArray(gVAO->getID());
		glDrawArrays(GL_PATCHES, 0, patches.size() * 16);
		glBindVertexArray(0);

	gBezierShader->disable();
	

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)

	glPolygonMode(GL_BACK, GL_LINE);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	gStack = new MatrixStack(16);

	gStack->loadIdentity();
	gStack->perspective(45, gWidth /gHeight, 0.1, 100);
	gProjection = gStack->current();

	gStack->loadIdentity();
	gStack->translate(0, 0, zCam);
	gStack->rotate(rotX, 1, 0, 0);
	gStack->rotate(rotY, 0, 1, 0);
	gModelView = gStack->current();

	GL_ERROR_CHECK

}

void InitBuffers() {
	WFObject * obj = WFObject::FromFile(MESH);
	ComputePatches(obj);
	delete obj;

	gVAO = new VertexArrayObject(1, GL_STATIC_DRAW);
	gVAO->setBufferData(0, sizeof(Bezier4x4) * patches.size(), &patches[0]);
	gVAO->setVertexAttribute(0, 0, 3, GL_FLOAT, false, 24, 0);
	gVAO->setVertexAttribute(0, 1, 3, GL_FLOAT, false, 24,(void*)12);
	gVAO->enableVertexAttribute(0);
	gVAO->enableVertexAttribute(1);

	GL_ERROR_CHECK
}

void InitShaders() {
	gBezierShader = ShaderManager::getDefaultManager()->createShader("bezier.vert", "bezier.tsco", "bezier.tsev", "bezier.geom", "bezier.frag");	
	gNormalShader = ShaderManager::getDefaultManager()->createShader("bezier.vert", "bezier.tsco", "bezier.tsev", "bezier_normals.geom", "bezier_normals.frag");
	gLineShader = ShaderManager::getDefaultManager()->createShader("line.vert", "line.frag");
}

void ComputeCornerPoints(int currentPatch, vector<Bezier4x4Desc>& patches) {
	vector<Bezier4x4Desc> neighbours;
	vector<vec3> points;
	vec3 corner, average;
	
	// Bottom left
	points.clear();
	
	corner = patches[currentPatch].OriginalQuad[0];
	GetPatchesSharingCorner(currentPatch, 0, patches, neighbours);

	for(int i = 0; i < neighbours.size(); i++) {
		int ind = FindCorner(neighbours[i], corner);
		GetCornerNeighbours(neighbours[i], ind, points);
	}
	

	average = vec3();

	for(int i = 0; i < points.size(); i++)
		average = average + points[i];
	
	patches[currentPatch].ControlPoints[0] = scale(average, 1.0f / points.size());

	// Bottom roght
	points.clear();
	
	corner = patches[currentPatch].OriginalQuad[1];
	GetPatchesSharingCorner(currentPatch, 1, patches, neighbours);

	for(int i = 0; i < neighbours.size(); i++) {
		int ind = FindCorner(neighbours[i], corner);
		GetCornerNeighbours(neighbours[i], ind, points);
	}

	average = vec3();

	for(int i = 0; i < points.size(); i++)
		average = average + points[i];
	
	patches[currentPatch].ControlPoints[3] = scale(average, 1.0f / points.size());

	// Top right
	points.clear();
	
	corner = patches[currentPatch].OriginalQuad[2];
	GetPatchesSharingCorner(currentPatch, 2, patches, neighbours);

	for(int i = 0; i < neighbours.size(); i++) {
		int ind = FindCorner(neighbours[i], corner);
		GetCornerNeighbours(neighbours[i], ind, points);
	}

	average = vec3();

	for(int i = 0; i < points.size(); i++)
		average = average + points[i];
	
	patches[currentPatch].ControlPoints[15] = scale(average, 1.0f / points.size());

	// Top left
	points.clear();
	
	corner = patches[currentPatch].OriginalQuad[3];
	GetPatchesSharingCorner(currentPatch, 3, patches, neighbours);

	for(int i = 0; i < neighbours.size(); i++) {
		int ind = FindCorner(neighbours[i], corner);
		GetCornerNeighbours(neighbours[i], ind, points);
	}

	average = vec3();

	for(int i = 0; i < points.size(); i++)
		average = average + points[i];
	
	patches[currentPatch].ControlPoints[12] = scale(average, 1.0f / points.size());
	
}

void GetPatchesSharingCorner(int currentPatch, int corner, vector<Bezier4x4Desc>& patches, vector<Bezier4x4Desc>& neighbours) {
	neighbours.clear();

	for(int i = 0; i < patches.size(); i++) {
		for(int j = 0; j < 4; j++) {
			if(patches[currentPatch].OriginalQuad[corner] == patches[i].OriginalQuad[j]) {
				neighbours.push_back(patches[i]);
			}
		}
	}
}

int FindCorner(Bezier4x4Desc& patch, vec3 corner) {
	for(int i = 0; i < 4; i++) {
		if(patch.OriginalQuad[i] == corner)
			return i;
	}

	throw "Corner not found!";
}

void GetCornerNeighbours(Bezier4x4Desc& patch, int corner, vector<vec3>& neighbours) {
	int i1 = -1, i2 = -1;
	switch(corner) {
	case 0: i1 = 1; i2 = 4; break;
	case 1: i1 = 2; i2 = 7; break;
	case 2: i1 = 11; i2 = 14; break;
	case 3: i1 = 8; i2 = 13; break;
	default:
		throw "Invalid corner: " + corner;
	}
	/*
	vec3 v = scale(patch.ControlPoints[i1] + patch.ControlPoints[i2], 0.5f);

	neighbours.push_back(v);*/

	if(!Contains(neighbours, patch.ControlPoints[i1]))
		neighbours.push_back(patch.ControlPoints[i1]);
	
	if(!Contains(neighbours, patch.ControlPoints[i2]))
		neighbours.push_back(patch.ControlPoints[i2]);
}

int FindFaceByEdge(vector<WFFace*>* faces, int currentFace, int i1, int i2) {

	for(int i = 0; i < faces->size(); i++) {
		WFFace* current = faces->at(i);

		if(i != currentFace) {
			for(int j = 0; j < 4; j++) {
				int j1 = j;
				int j2 = (j + 1) % 4;

				if(
					(current->GetVertexIndex(j1) == i1 && current->GetVertexIndex(j2) == i2) ||
					(current->GetVertexIndex(j1) == i2 && current->GetVertexIndex(j2) == i1))
					return i;

			}
		}
	}

	return -1;
}

Bezier4x4Desc ComputeAdjacency(Bezier4x4Desc curPatch, Bezier4x4Desc adjPatch, int vc0, int vc1, int va0, int va1) {
	
	Bezier4x4Desc result = adjPatch;

	for(int i = 0; i < 4; i++) {
		if(curPatch.OriginalQuad[vc0] == result.OriginalQuad[va0] &&
			curPatch.OriginalQuad[vc1] == result.OriginalQuad[va1])
			return result;
		else
			result = result.Rotate();		
	}
}

Bezier4x4Desc ComputeAdjRight(Bezier4x4Desc curPatch) {
	return ComputeAdjacency(curPatch, *(curPatch.adjRight), 1, 2, 0, 3);
}

Bezier4x4Desc ComputeAdjLeft(Bezier4x4Desc curPatch) {
	return ComputeAdjacency(curPatch, *(curPatch.adjLeft), 0, 3, 1, 2);
}

Bezier4x4Desc ComputeAdjBottom(Bezier4x4Desc curPatch) {
	return ComputeAdjacency(curPatch, *(curPatch.adjBottom), 0, 1, 3, 2);
}

Bezier4x4Desc ComputeAdjTop(Bezier4x4Desc curPatch) {
	return ComputeAdjacency(curPatch, *(curPatch.adjTop), 3, 2, 0, 1);
}



/* A partire da un file WaveFront, genera le patch */
void ComputePatches(WFObject *obj) {
	// Descrittori delle patch da generare
	vector<Bezier4x4Desc> patchesDesc;

	// Facce (quadrilateri!) dell'oggetto
	vector<WFFace*> * faces = obj->GetFaces();

	// Vertici dell'oggetto
	vector<vec3> * vertices = obj->GetVertices();

	// Normali dell'oggetto
	vector<vec3> * normals = obj->GetNormals();

	/*
		Per ogni quadrilatero creiamo un descrittore di patch (Bezier4x4Desc), 
	*/
	for(int i = 0; i < faces->size(); i++) {
		Bezier4x4Desc desc;
		WFFace * current = faces->at(i);
		/* Andiamo ad impostare i vertici originali nel campo OriginalQuad[0...3] */
		desc.OriginalQuad[0] = desc.ControlPoints[0] = vertices->at(current->GetVertexIndex(0));
		desc.OriginalQuad[1] = desc.ControlPoints[3] = vertices->at(current->GetVertexIndex(1));
		desc.OriginalQuad[2] = desc.ControlPoints[15] = vertices->at(current->GetVertexIndex(2));
		desc.OriginalQuad[3] = desc.ControlPoints[12] = vertices->at(current->GetVertexIndex(3));

		/* Andiamo ad impostare le normali nel campo Normals[0...3] */
		desc.Normals[0] = normals->at(current->GetNormalIndex(0));
		desc.Normals[1] = normals->at(current->GetNormalIndex(1));
		desc.Normals[2] = normals->at(current->GetNormalIndex(2));
		desc.Normals[3] = normals->at(current->GetNormalIndex(3));

		/* Calcoliamo i punti centrali della patch */
		desc.ControlPoints[5] = lerp(vertices->at(current->GetVertexIndex(0)), vertices->at(current->GetVertexIndex(2)), 1.0f / 3.0f);
		desc.ControlPoints[6] = lerp(vertices->at(current->GetVertexIndex(1)), vertices->at(current->GetVertexIndex(3)), 1.0f / 3.0f);
		desc.ControlPoints[9] = lerp(vertices->at(current->GetVertexIndex(1)), vertices->at(current->GetVertexIndex(3)), 2.0f / 3.0f);
		desc.ControlPoints[10] = lerp(vertices->at(current->GetVertexIndex(0)), vertices->at(current->GetVertexIndex(2)), 2.0f / 3.0f);

		patchesDesc.push_back(desc);

	}
	/*
		Per ogni patch calcoliamo le patch adiacenti in tutte le direzione ed eventualmente
		i punti di controlli sui lati 
	*/
	for(int i = 0; i < faces->size(); i++) {
		int faceIndex = -1;
		WFFace *current = faces->at(i);
		Bezier4x4Desc* currentDesc = &patchesDesc[i];

		/* Calcoliamo le adianceze */

		// Adjacency bottom
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(0), current->GetVertexIndex(1));
		currentDesc->adjBottom = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Adjacency right
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(1), current->GetVertexIndex(2));
		currentDesc->adjRight = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Adjacency top
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(2), current->GetVertexIndex(3));
		currentDesc->adjTop = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Adjacency left
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(3), current->GetVertexIndex(0));
		currentDesc->adjLeft = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		/*
			Calcoliamo i punti di controllo sui lati
		*/

		if(currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adj = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[1] = lerp(currentDesc->ControlPoints[5], adj.ControlPoints[9], 0.5f);
			currentDesc->ControlPoints[2] = lerp(currentDesc->ControlPoints[6], adj.ControlPoints[10], 0.5f);

		} else {
			currentDesc->ControlPoints[1] = lerp(currentDesc->ControlPoints[0], currentDesc->ControlPoints[3], 1.0f / 3.0f);
			currentDesc->ControlPoints[2] = lerp(currentDesc->ControlPoints[0], currentDesc->ControlPoints[3], 2.0f / 3.0f);
		}

		if(currentDesc->adjTop != NULL) {
			Bezier4x4Desc adj = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[13] = lerp(currentDesc->ControlPoints[9], adj.ControlPoints[5], 0.5f);
			currentDesc->ControlPoints[14] = lerp(currentDesc->ControlPoints[10], adj.ControlPoints[6], 0.5f);
		} else {
			currentDesc->ControlPoints[13] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[15], 1.0f / 3.0f);
			currentDesc->ControlPoints[14] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[15], 2.0f / 3.0f);
		}

		if(currentDesc->adjRight != NULL) {
			Bezier4x4Desc adj = ComputeAdjRight(*currentDesc);
			currentDesc->ControlPoints[7] = lerp(currentDesc->ControlPoints[6], adj.ControlPoints[5], 0.5f);
			currentDesc->ControlPoints[11] = lerp(currentDesc->ControlPoints[10], adj.ControlPoints[9], 0.5f);
		} else {
			currentDesc->ControlPoints[7] = lerp(currentDesc->ControlPoints[3], currentDesc->ControlPoints[15], 1.0f / 3.0f);
			currentDesc->ControlPoints[11] = lerp(currentDesc->ControlPoints[3], currentDesc->ControlPoints[15], 2.0f / 3.0f);
		}

		if(currentDesc->adjLeft != NULL) {
			Bezier4x4Desc adj = ComputeAdjLeft(*currentDesc);
			currentDesc->ControlPoints[4] = lerp(currentDesc->ControlPoints[5], adj.ControlPoints[6], 0.5f);
			currentDesc->ControlPoints[8] = lerp(currentDesc->ControlPoints[9], adj.ControlPoints[10], 0.5f);
		} else {
			currentDesc->ControlPoints[4] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[0], 1.0f / 3.0f);
			currentDesc->ControlPoints[8] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[0], 2.0f / 3.0f);
		}

	}


	/*
		A questo punto, per ogni descrittore andiamo a calcolare i punti di controllo sugli 
		angoli come media dei punti attorno
	*/

	for(int i = 0; i < faces->size(); i++)
		ComputeCornerPoints(i, patchesDesc);
	
	/*
		Proiettiamo i punti della patch sui 4 piani definiti dalle 4 normali ai vertici
	*/
	for(int i = 0; i < faces->size(); i++) {
		
		Bezier4x4Desc * currentDesc = &patchesDesc[i];

		// Bottom Left
		currentDesc->ControlPoints[1] = project(currentDesc->ControlPoints[1], currentDesc->ControlPoints[0], currentDesc->Normals[0]);
		currentDesc->ControlPoints[4] = project(currentDesc->ControlPoints[4], currentDesc->ControlPoints[0], currentDesc->Normals[0]);
		currentDesc->ControlPoints[5] = project(currentDesc->ControlPoints[5], currentDesc->ControlPoints[0], currentDesc->Normals[0]);

		// Bottom Right
		currentDesc->ControlPoints[2] = project(currentDesc->ControlPoints[2], currentDesc->ControlPoints[3], currentDesc->Normals[1]);
		currentDesc->ControlPoints[6] = project(currentDesc->ControlPoints[6], currentDesc->ControlPoints[3], currentDesc->Normals[1]);
		currentDesc->ControlPoints[7] = project(currentDesc->ControlPoints[7], currentDesc->ControlPoints[3], currentDesc->Normals[1]);

		// Top Right
		currentDesc->ControlPoints[10] = project(currentDesc->ControlPoints[10], currentDesc->ControlPoints[15], currentDesc->Normals[2]);
		currentDesc->ControlPoints[11] = project(currentDesc->ControlPoints[11], currentDesc->ControlPoints[15], currentDesc->Normals[2]);
		currentDesc->ControlPoints[14] = project(currentDesc->ControlPoints[14], currentDesc->ControlPoints[15], currentDesc->Normals[2]);

		// Top Left
		currentDesc->ControlPoints[8] = project(currentDesc->ControlPoints[8], currentDesc->ControlPoints[12], currentDesc->Normals[3]);
		currentDesc->ControlPoints[9] = project(currentDesc->ControlPoints[9], currentDesc->ControlPoints[12], currentDesc->Normals[3]);
		currentDesc->ControlPoints[13] = project(currentDesc->ControlPoints[13], currentDesc->ControlPoints[12], currentDesc->Normals[3]);
	}

	/*
		Generiamo le patch
	*/
	for(int i = 0; i < patchesDesc.size(); i++) {
		patches.push_back(Bezier4x4(patchesDesc.at(i)));
	}
	
}



void InitTextures() {

}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	int b[2];
	switch(key) {
	case '+':
		gTessLevel += 0.1f;
		break;
	case '-':
		gTessLevel -= 0.1f;
		break;
	case '1':
		zCam += 0.1f;
		break;
	case '2':
		zCam -= 0.1f;
		break;
	case 'p':
		showPoints = !showPoints;
		break;
	case 'n':
		interpolatedNormals = !interpolatedNormals;
		break;
	case 's':
		showNormals = !showNormals;
		break;
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE, b);
		if(b[0] == GL_LINE)
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case VK_ESCAPE:
		exit(0);
		break;
	}

	if(gTessLevel < 1.0f)
		gTessLevel = 1.0f;
	else if(gTessLevel > 64.0f)
		gTessLevel = 64.0f;
}

void MouseMotion(int x, int y) {
	if(dragging) {
		float dx = x - prevX;
		float dy = y - prevY;
		rotX += (dy / gWidth) * PI;
		rotY += (dx / gHeight) * PI;
		prevX = x;
		prevY = y;
	}
}
void Mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) 
		{
			prevX = x;
			prevY = y;
			dragging = true;
		}
		else
			dragging = false;
	}
}

void PngLoad(const char* filename) {
 
	unsigned int width, height;
	std::vector<unsigned char> *image = new std::vector<unsigned char>;
	unsigned error = LodePNG::decode(*image, width, height, filename);
	unsigned char* data = new unsigned char[width*height*4];

	for(unsigned int i = 0; i < width * height * 4; i++)
		data[i] = (*image)[i];

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete image;
	delete data;
}


GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter) {
	GLuint result;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	PngLoad(fileName);

	if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return result;

}