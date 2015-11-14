#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

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
#define MESH "cube.obj"

struct Vertex {
	vec3 Position;
};

struct Bezier4x4Desc {
	vec3 ControlPoints[16];
	vec3 OriginalQuad[4];
	Bezier4x4Desc* adjLeft;
	Bezier4x4Desc* adjRight;
	Bezier4x4Desc* adjTop;
	Bezier4x4Desc* adjBottom;


	Bezier4x4Desc Rotate() {
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
	/*
	void toString() {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				cout << ControlPoints[i * 4 + j].x << " ";
			}
			cout << "\n";
		}
		for(int i = 0; i < 2; i++) {
			for(int j = 0; j < 2; j++) {
				cout << OriginalQuad[i * 2 + j].x << " "<< OriginalQuad[i * 2 + j].y << " "<< OriginalQuad[i * 2 + j].z << "\n";
			}
		}
		cout << "\n--------------------------------\n";
	}*/

};

struct Bezier4x4 {
	vec3 ControlPoints[16];

	Bezier4x4() {
		for(int i = 0; i <  16; i++)
			ControlPoints[i] = vec3();
	}

	Bezier4x4(Bezier4x4Desc d) {
		for(int i = 0; i <  16; i++)
			ControlPoints[i] = d.ControlPoints[i];
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
vec3 ComputeCornerPoint(vec3 p1, vec3 p2, vec3 p3, vec3 p4);
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);
void MouseMotion(int, int);
void Mouse(int, int, int, int);
void PngLoad(const char* filename);
GLuint PngTexture(const char * fileName, GLint minFilter, GLint magFilter);

MatrixStack *gStack;
Shader *gBezierShader,*gLineShader;
VertexArrayObject *gVAO;
vector<Bezier4x4> patches;
vector<vec3> quadGrid;

mat4 gModelView, gProjection;

float gWidth = 640, gHeight = 480;
float gTessLevel = 2.0f;
float rotX = 0.0f, rotY = 0.0f;
float prevX, prevY;
float zCam = -8;

bool dragging = false;

int activePatch = 0;

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

	glClearColor(0,0,0.5,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	gStack->loadIdentity();
	gStack->translate(0, 0, zCam);
	gStack->rotate(rotX, 1, 0, 0);
	gStack->rotate(rotY, 0, 1, 0);
	gModelView = gStack->current();

	/*
	gLineShader->enable();
		gBezierShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
		gBezierShader->setUniformMatrix("in_ModelViewMatrix", gModelView);

		
		glPointSize(5);
		glBindVertexArray(gVAO->getID());
		glDrawArrays(GL_POINTS, 0, patches.size() * 16);
		glBindVertexArray(0);

	gLineShader->disable();*/

	
	gBezierShader->enable();

		gBezierShader->setUniformMatrix("in_ProjectionMatrix", gProjection);
		gBezierShader->setUniformMatrix("in_ModelViewMatrix", gModelView);
		gBezierShader->setUniform("in_TessLevel", gTessLevel);

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
	gVAO->setVertexAttribute(0, 0, 3, GL_FLOAT);
	gVAO->enableVertexAttribute(0);

	GL_ERROR_CHECK
}

void InitShaders() {
	gBezierShader = ShaderManager::getDefaultManager()->createShader("bezier.vert", "bezier.tsco", "bezier.tsev", "bezier.frag");
	gLineShader = ShaderManager::getDefaultManager()->createShader("line.vert", "line.frag");
}

vec3 ComputeCornerPoint(vec3 p1, vec3 p2, vec3 p3, vec3 p4) {
	vector<vec3> v, v2;
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);


	for(int i = 0; i < v.size(); i++) {
		bool add = true;

		for(int j = 0; j < v2.size(); j++) {
			if(v[i] == v2[j]) {
				add = false;
				break;
			}
		}

		if(add)
			v2.push_back(v[i]);
	}
	
	vec3 result;

	for(int i = 0; i < v2.size(); i++) 
		result = result + v2[i];
	return scale(result, 1.0f / v2.size());
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
		/*
		if(i != currentFace && current->ContainsVertexIndex(i1) && current->ContainsVertexIndex(i2))
			return i;*/
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

void ComputePatches(WFObject *obj) {
	vector<Bezier4x4Desc> patchesDesc;
	vector<WFFace*> * faces = obj->GetFaces();
	vector<vec3> * vertices = obj->GetVertices();

	cout << "Patches to generate: " << faces->size() << endl;

	for(int i = 0; i < faces->size(); i++) {
		Bezier4x4Desc desc;
		WFFace * current = faces->at(i);

		desc.OriginalQuad[0] = desc.ControlPoints[0] = vertices->at(current->GetVertexIndex(0));
		desc.OriginalQuad[1] = desc.ControlPoints[3] = vertices->at(current->GetVertexIndex(1));
		desc.OriginalQuad[2] = desc.ControlPoints[15] = vertices->at(current->GetVertexIndex(2));
		desc.OriginalQuad[3] = desc.ControlPoints[12] = vertices->at(current->GetVertexIndex(3));

		desc.ControlPoints[5] = lerp(vertices->at(current->GetVertexIndex(0)), vertices->at(current->GetVertexIndex(2)), 1.0f / 3.0f);
		desc.ControlPoints[6] = lerp(vertices->at(current->GetVertexIndex(1)), vertices->at(current->GetVertexIndex(3)), 1.0f / 3.0f);
		desc.ControlPoints[9] = lerp(vertices->at(current->GetVertexIndex(1)), vertices->at(current->GetVertexIndex(3)), 2.0f / 3.0f);
		desc.ControlPoints[10] = lerp(vertices->at(current->GetVertexIndex(0)), vertices->at(current->GetVertexIndex(2)), 2.0f / 3.0f);

		patchesDesc.push_back(desc);

	}

	for(int i = 0; i < faces->size(); i++) {
		int faceIndex = -1;
		WFFace *current = faces->at(i);
		Bezier4x4Desc* currentDesc = &patchesDesc[i];

		// Bottom adjacency
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(0), current->GetVertexIndex(1));
		currentDesc->adjBottom = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Right adjacency
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(1), current->GetVertexIndex(2));
		currentDesc->adjRight = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Top adjacency
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(2), current->GetVertexIndex(3));
		currentDesc->adjTop = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Left adjacency
		faceIndex = FindFaceByEdge(faces, i, current->GetVertexIndex(3), current->GetVertexIndex(0));
		currentDesc->adjLeft = faceIndex != -1 ? &patchesDesc[faceIndex] : NULL;

		// Bottom points
		if(currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adj = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[1] = lerp(currentDesc->ControlPoints[5], adj.ControlPoints[9], 0.5f);
			currentDesc->ControlPoints[2] = lerp(currentDesc->ControlPoints[6], adj.ControlPoints[10], 0.5f);

		} else {
			currentDesc->ControlPoints[1] = lerp(currentDesc->ControlPoints[0], currentDesc->ControlPoints[3], 1.0f / 3.0f);
			currentDesc->ControlPoints[2] = lerp(currentDesc->ControlPoints[0], currentDesc->ControlPoints[3], 2.0f / 3.0f);
		}

		// Top points
		if(currentDesc->adjTop != NULL) {
			Bezier4x4Desc adj = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[13] = lerp(currentDesc->ControlPoints[9], adj.ControlPoints[5], 0.5f);
			currentDesc->ControlPoints[14] = lerp(currentDesc->ControlPoints[10], adj.ControlPoints[6], 0.5f);
		} else {
			currentDesc->ControlPoints[13] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[15], 1.0f / 3.0f);
			currentDesc->ControlPoints[14] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[15], 2.0f / 3.0f);
		}

		// Right points
		if(currentDesc->adjRight != NULL) {
			Bezier4x4Desc adj = ComputeAdjRight(*currentDesc);
			currentDesc->ControlPoints[7] = lerp(currentDesc->ControlPoints[6], adj.ControlPoints[5], 0.5f);
			currentDesc->ControlPoints[11] = lerp(currentDesc->ControlPoints[10], adj.ControlPoints[9], 0.5f);
		} else {
			currentDesc->ControlPoints[7] = lerp(currentDesc->ControlPoints[3], currentDesc->ControlPoints[15], 1.0f / 3.0f);
			currentDesc->ControlPoints[11] = lerp(currentDesc->ControlPoints[3], currentDesc->ControlPoints[15], 2.0f / 3.0f);
		}

		// Left points
		if(currentDesc->adjLeft != NULL) {
			Bezier4x4Desc adj = ComputeAdjLeft(*currentDesc);
			currentDesc->ControlPoints[4] = lerp(currentDesc->ControlPoints[5], adj.ControlPoints[6], 0.5f);
			currentDesc->ControlPoints[8] = lerp(currentDesc->ControlPoints[9], adj.ControlPoints[10], 0.5f);
		} else {
			currentDesc->ControlPoints[4] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[0], 1.0f / 3.0f);
			currentDesc->ControlPoints[8] = lerp(currentDesc->ControlPoints[12], currentDesc->ControlPoints[0], 2.0f / 3.0f);
		}

	}


	// Corner points
	
	
	for(int i = 0; i < faces->size(); i++) {

		int k = 0;
		vec3 average;

		// Bottom right point
		Bezier4x4Desc* currentDesc = &patchesDesc[i];

		if(currentDesc->adjRight != NULL && currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adjRight = ComputeAdjRight(*currentDesc);
			Bezier4x4Desc adjBottom = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[3] = ComputeCornerPoint(currentDesc->ControlPoints[2], adjRight.ControlPoints[1], 
				currentDesc->ControlPoints[7], adjBottom.ControlPoints[11]);
		} else if(currentDesc->adjRight != NULL) {
			Bezier4x4Desc adjRight = ComputeAdjRight(*currentDesc);
			currentDesc->ControlPoints[3] = lerp(currentDesc->ControlPoints[2], adjRight.ControlPoints[1], 0.5f);
		} else if(currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adjBottom = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[3] = lerp(currentDesc->ControlPoints[7], adjBottom.ControlPoints[11], 0.5f);
		}

		// Bottom left point

		if(currentDesc->adjLeft != NULL && currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adjLeft = ComputeAdjLeft(*currentDesc);
			Bezier4x4Desc adjBottom = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[0] = ComputeCornerPoint(currentDesc->ControlPoints[1], adjLeft.ControlPoints[2],
				currentDesc->ControlPoints[4], adjBottom.ControlPoints[8]);
		
		} else if(currentDesc->adjLeft != NULL) {
			Bezier4x4Desc adjLeft = ComputeAdjLeft(*currentDesc);
			currentDesc->ControlPoints[0] = lerp(currentDesc->ControlPoints[1], adjLeft.ControlPoints[2], 0.5f);
		} else if(currentDesc->adjBottom != NULL) {
			Bezier4x4Desc adjBottom = ComputeAdjBottom(*currentDesc);
			currentDesc->ControlPoints[0] = lerp(currentDesc->ControlPoints[4], adjBottom.ControlPoints[8], 0.5f);
		}

		// Top left point
		if(currentDesc->adjLeft != NULL && currentDesc->adjTop != NULL) {
			Bezier4x4Desc adjLeft = ComputeAdjLeft(*currentDesc);
			Bezier4x4Desc adjTop = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[12] = ComputeCornerPoint(currentDesc->ControlPoints[13], adjLeft.ControlPoints[14],
				currentDesc->ControlPoints[8], adjTop.ControlPoints[4]);
		} else if(currentDesc->adjLeft != NULL) {
			Bezier4x4Desc adjLeft = ComputeAdjLeft(*currentDesc);
			currentDesc->ControlPoints[12] = lerp(currentDesc->ControlPoints[13], adjLeft.ControlPoints[14], 0.5f);
		} else if(currentDesc->adjTop != NULL) {
			Bezier4x4Desc adjTop = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[12] = lerp(currentDesc->ControlPoints[8], adjTop.ControlPoints[4], 0.5f);
		}

		// Top right point
		if(currentDesc->adjRight != NULL && currentDesc->adjTop != NULL) {
			Bezier4x4Desc adjRight = ComputeAdjRight(*currentDesc);
			Bezier4x4Desc adjTop = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[15] = ComputeCornerPoint(currentDesc->ControlPoints[14], adjRight.ControlPoints[13],
				currentDesc->ControlPoints[11], adjTop.ControlPoints[7]);

		} else if(currentDesc->adjRight != NULL) {
			Bezier4x4Desc adjRight = ComputeAdjRight(*currentDesc);
			currentDesc->ControlPoints[15] = lerp(currentDesc->ControlPoints[14], adjRight.ControlPoints[13], 0.5f);
		} else if(currentDesc->adjTop != NULL) {
			Bezier4x4Desc adjTop = ComputeAdjTop(*currentDesc);
			currentDesc->ControlPoints[15] = lerp(currentDesc->ControlPoints[11], adjTop.ControlPoints[7], 0.5f);
		}
	}

	
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
	case '3':
		activePatch = (activePatch + 1) % patches.size();
		break;
	case 'w':
		glGetIntegerv(GL_POLYGON_MODE, b);
		if(b[0] == GL_LINE)
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);
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