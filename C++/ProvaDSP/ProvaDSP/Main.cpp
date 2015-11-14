#include <iostream>
#include "bass\bass.h"
#include "gl\glut.h"

using namespace std;

#define FFT_W 128

void InitGL();
void Render();
void Reshape(int, int);
void Keyboard(unsigned char, int, int);

HSTREAM stream;
float fftResult[FFT_W];
float ww = 640, wh = 480;
float * streamData;
BASS_CHANNELINFO streamInfo;
__int64 lengthBytes;
__int64 lengthSamples;


void CheckError() {
	int err = BASS_ErrorGetCode();
	if(err != BASS_OK)
		std::cout << "Error: " << err << std::endl;
}

void fft(float * samples, int length, float * result) {
	static const float PI = 3.141592f;

	for(int i = 0; i < length; i++) {

		result[i] = 0;

		for(int k = 0; k < length; k++) {
			float angle = (float)i * PI * (float)k / (float)length;
			result[i] += samples[k] * sin(angle);
		}
	}
}

DWORD CALLBACK StreamProc(HSTREAM handle, void * buffer, DWORD length, void * user) {
	static int pointer = 0;
	static int fftPointer = 0;
	static float fftBuffer[FFT_W];

	float * outBuffer = (float*)buffer;
	int outLength = length / sizeof(float);

	for(int i = 0; i < outLength; i++) {
		if(fftPointer % FFT_W == 0) {
			fft(fftBuffer, FFT_W, fftResult);
			fftPointer = 0;
		}
		//if((pointer + i) % 256 == 0)
		//	BASS_ChannelGetData(stream, fftResult, BASS_DATA_FFT2048);

		outBuffer[i] = streamData[(pointer + i) % lengthSamples];
		fftBuffer[fftPointer++] = streamData[(pointer + i) % lengthSamples];
	}
	pointer = (pointer + outLength) % lengthSamples;

	return outLength * sizeof(float);
}

int main(int argc, char ** argv) {

	// prima di TUTTO (TUTTO TUTTO TUTTO) creare la finestra sennò non funziona una mazza
	glutInit(&argc, argv);
	// rgba mode, double buffering, depth buffering, stencil buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	// window' top left corner position
	glutInitWindowPosition(0,0);
	// window's size
	glutInitWindowSize(640, 480);
	// create window
	glutCreateWindow("Window Title");
	// Finestra creata... Adesso dovrebbe andare, ma

	for(int i = 0; i < FFT_W; i++) {
		fftResult[i] = 0;
	}
	
	InitGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);
	glutKeyboardFunc(Keyboard);

	BASS_Init(1, 44100, BASS_DEVICE_SPEAKERS, 0, NULL);



	stream = BASS_StreamCreateFile(FALSE, "BridgeZone.mp3", 0, 0, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);

	BASS_ChannelGetInfo(stream, &streamInfo);

	cout << "Freq: " << streamInfo.freq << endl;
	cout << "Chanels: " << streamInfo.chans << endl;
	cout << "Float: " << ((streamInfo.flags & BASS_SAMPLE_FLOAT) > 0) << endl;
	cout << "8 bits: " << ((streamInfo.flags & BASS_SAMPLE_8BITS) > 0) << endl;

	lengthBytes = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
	lengthSamples = lengthBytes / sizeof(float);
	streamData = new float[lengthSamples];

	BASS_ChannelGetData(stream, streamData, lengthBytes);
	
	cout << "Samples: " << lengthSamples << endl;

	BASS_StreamFree(stream);
	
	stream = BASS_StreamCreate(streamInfo.freq, streamInfo.chans, BASS_SAMPLE_FLOAT, StreamProc, NULL);

	BASS_ChannelPlay(stream, TRUE);

	CheckError();

	glutMainLoop();
}

// called when window is resized
void Reshape(int w, int h) {
	glViewport(0,0,w,h); // viewport resize
	ww = w;
	wh = h;
}

// called when window is drawn
void Render() {
	
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	glOrtho(0, ww, 0, wh, -1, 1); 

	// projection transform

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < FFT_W / 4; i++) {
		//glVertex3f((float)i * ww / (float)FFT_W, 0, 0);
		glVertex3f((float)i * ww / (float)FFT_W * 4, (float)fftResult[i] + wh / 2, 0);
	}
	glEnd();

	// model-view transform

	glutSwapBuffers(); // swap backbuffer with frontbuffer
}

void InitGL() {
	// Init opengl(depth test, blending, lighting and so on...)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0);
	
}

// Called by keyboard events
void Keyboard(unsigned char key, int x, int y) {
	
}