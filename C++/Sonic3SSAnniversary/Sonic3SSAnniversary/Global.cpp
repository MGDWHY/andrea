#include "Global.h"

namespace Global {

	ThreadStats::ThreadStats() {
		this->frequency = 0;
		this->time = 0;
	}

	float Random(float x) {
		return (float) rand() / RAND_MAX * x;
	}

	float Random(float min, float max) {
		return min + Global::Random(max - min);
	}

	int DisplayMode::operator==(const DisplayMode &other) {
		return this->width == other.width &&
			this->height == other.height &&
			this->frequency == other.frequency &&
			this->bitsPerPixel == other.bitsPerPixel;
	}

	vector<DisplayMode> GetAvailableDisplayModes(int minFrequency, int bpp) {
		DEVMODE m_data;
	
		m_data.dmSize = sizeof(DEVMODE);
		m_data.dmDriverExtra = 0;

		vector<DisplayMode> modes;

		for(DWORD i = 0; EnumDisplaySettings(NULL, i, &m_data); i++) {
			if(m_data.dmDisplayFrequency >= minFrequency && m_data.dmBitsPerPel == bpp) {
				DisplayMode mode;
				mode.width = m_data.dmPelsWidth;
				mode.height = m_data.dmPelsHeight;
				mode.frequency = m_data.dmDisplayFrequency;
				mode.bitsPerPixel = m_data.dmBitsPerPel;
				if(find(modes.begin(), modes.end(), mode) == modes.end())
					modes.push_back(mode);
			}
		}

		return modes;
	}

	void CollectGameStats(GameStats& gameStats, int executionTime, bool incFps, bool incLogic) {
		static Timer * timer = new Timer();
		static int time = 0;
		static GameStats current;

		if(incFps) {
			current.fps.frequency++;
			current.fps.time = current.fps.time > executionTime ? current.fps.time : executionTime;
		}
			

		if(incLogic) {
			current.logic.frequency++;
			current.logic.time = current.logic.time > executionTime ? current.logic.time : executionTime;
		}


		time += timer->dtMillis();

		if(time >= 1000) {
			gameStats = current;
			current = GameStats();
			time = 0;
		}
	}

	GameConfig LoadGameConfig() {

		ifstream is;
		GameConfig result;

		is.open(CONFIG_FILE, ios_base::binary);

		if(!is.good()) {
			#ifdef GL_UTILS_LOG_ENABLED
				Logger::getDefaultLogger()->writeMessage(1, "Global::LoadGameConfig()", "Configuration files not found. Loading default configuration.");
			#endif

			result.displayMode = GetAvailableDisplayModes(DM_MIN_FREQUENCY, DM_BITS_PER_PIXEL).back();
			result.windowed = DEFAULT_WINDOWED;
			result.quality = DEFAULT_GRAPHICS_QUALITY;
			result.anisotropicFilter = DEFAULT_ANISOTROPIC;
			result.stereo3D = DEFAULT_STEREO3D;
			result.shadowsEnabled = DEFAULT_SHADOWS_ENABLED;
			result.shadowQuality = DEFAULT_SHADOW_QUALITY;
			result.musicVolume = DEFAULT_MUSIC_VOLUME;
			result.fxVolume = DEFAULT_MUSIC_VOLUME;

		} else {
			is.read((char*)&result, sizeof(GameConfig));
		}

		is.close();

		#ifdef GL_UTILS_LOG_ENABLED
			Logger::getDefaultLogger()->writeMessage(0, "Global::LoadGameConfig()", "Configuration file loaded.");
		#endif

		Global::StoreGameConfig(result);
		
		return result;

	}

	void StoreGameConfig(const GameConfig& gameConfig) {
		ofstream os;

		os.open(CONFIG_FILE, ios_base::binary);

		os.write((char*)&gameConfig, sizeof(GameConfig));

		os.close();

	}

	void GetModuleDirectory(char * dirName) {
		char buffer[MAX_PATH-1];

		int length = GetModuleFileName(NULL, buffer, MAX_PATH-1);

		for(int i = length, done = false; !done; i--) {
			if(buffer[i] == '\\') {
				done = true;
				length = i;
				buffer[i + 1] = '\0';
			}
		}

		memcpy(dirName, buffer, length + 2); 
	}


	void CheckGLVersion() {
		int maj, min;
		glGetIntegerv(GL_MAJOR_VERSION, &maj);
		glGetIntegerv(GL_MINOR_VERSION, &min);

		if(maj < GL_MIN_MAJ_VERSION && min < GL_MIN_MIN_VERSION) {
			char buffer[200];
			sprintf(buffer, "Your OpenGL version (%d.%d) doesn't seem to satisfy the game's requirements.\n Do you want to launch the game anyway?", maj, min);
			int res = MessageBox(NULL, buffer,
				"OpenGL Version Check", MB_YESNO | MB_ICONQUESTION);

			if(res == IDNO)
				exit(0);
		}
		
	}

	vector<string> LoadStageFiles() {
		ifstream is;
		vector<string> result;

		is.open(STAGE_FILE, ios_base::in);

		#ifdef GL_UTILS_LOG_ENABLED
			if(!is.good())
				Logger::getDefaultLogger()->writeMessage(2, "Global::LoadGameConfig()", "Stage file not found");
		#endif
	
		int numStages = BinaryIO::Read<int>(is);
		for(int i = 0; i < numStages; i++) {
			int strLen = BinaryIO::Read<int>(is);
			char * buffer = new char[strLen];
			is.read(buffer, strLen);
			result.push_back(string(buffer, strLen));
		}

		is.close();

		return result;
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

		Global::PngLoad(fileName);

		if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST)
			glGenerateMipmap(GL_TEXTURE_2D);	

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Global::LoadGameConfig().anisotropicFilter);

		return result;

	}

	void ConvertModel(string wfFileName, string xboFileName) {
		WaveFront::WFObject * obj = WaveFront::WFObject::FromFile(wfFileName);
		ExtendedBinary3DObject::ToFile(obj, xboFileName);
		delete obj;
	}

	void ConvertModel(string wfFileName, string wfmatlibFileName, string xboFileName) {
		WaveFront::WFObject * obj = WaveFront::WFObject::FromFile(wfFileName);
		WaveFront::WFMaterialLibrary * matlib = WaveFront::WFMaterialLibrary::FromFile(wfmatlibFileName);
		ExtendedBinary3DObject::ToFile(obj, matlib, xboFileName);
		delete obj;
		delete matlib;
	}

	void ConvertModels() {
		#ifdef GL_UTILS_LOG_ENABLED
				Logger::getDefaultLogger()->writeMessage(0, "Global::ConvertModels()", "Converting models to XBO Format...");
		#endif
		ConvertModel("models/sonic0.obj", "models/sonic.mtl", "models/sonic0.xbo");
		ConvertModel("models/sonic1.obj", "models/sonic.mtl", "models/sonic1.xbo");
		ConvertModel("models/sonic2.obj", "models/sonic.mtl", "models/sonic2.xbo");
		ConvertModel("models/sonic3.obj", "models/sonic.mtl", "models/sonic3.xbo");
		ConvertModel("models/sonic4.obj", "models/sonic.mtl", "models/sonic4.xbo");
		ConvertModel("models/sonic5.obj", "models/sonic.mtl", "models/sonic5.xbo");
		ConvertModel("models/sonic6.obj", "models/sonic.mtl", "models/sonic6.xbo");
		ConvertModel("models/sonic7.obj", "models/sonic.mtl", "models/sonic7.xbo");
		ConvertModel("models/sonic8.obj", "models/sonic.mtl", "models/sonic8.xbo");
		ConvertModel("models/sonic9.obj", "models/sonic.mtl", "models/sonic9.xbo");
		ConvertModel("models/sonic10.obj", "models/sonic.mtl", "models/sonic10.xbo");
		ConvertModel("models/sonic11.obj", "models/sonic.mtl", "models/sonic11.xbo");
		ConvertModel("models/sonicJump.obj", "models/sonic.mtl", "models/sonicJump.xbo");
		ConvertModel("models/sonicStand.obj", "models/sonic.mtl", "models/sonicStand.xbo");
		ConvertModel("models/sphere.obj", "models/sphere.xbo");
		ConvertModel("models/sphereLow.obj", "models/sphereLow.xbo");
		ConvertModel("models/emerald.obj", "models/emerald.xbo");
		ConvertModel("models/ring.obj", "models/ring.xbo");
		ConvertModel("models/emerald2.obj", "models/emerald2.xbo");
		#ifdef GL_UTILS_LOG_ENABLED
				Logger::getDefaultLogger()->writeMessage(0, "Global::ConvertModels()", "Done");
		#endif
	}

	VertexArrayObject * CreateVertexArray(XBO * xbo, int shadingGroup) {
		VertexArrayObject * result = new VertexArrayObject(1, GL_STATIC_DRAW);
		GLuint * indices = new GLuint[xbo->ShadingGroups[shadingGroup].VerticesCount];

		for(GLuint i = 0; i < xbo->ShadingGroups[shadingGroup].VerticesCount; i++)
			indices[i] = i;

		result->setBufferData(0, sizeof(ExtendedBinary3DObject::Vertex) * xbo->ShadingGroups[shadingGroup].VerticesCount, 
			xbo->ShadingGroups[shadingGroup].Vertices);
		result->setElementsData(sizeof(GLuint), xbo->ShadingGroups[shadingGroup].VerticesCount, indices);

		result->setVertexAttribute(0, 0, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), 0);
		result->setVertexAttribute(0, 1, 3, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), (char*)0 + 12);
		result->setVertexAttribute(0, 2, 2, GL_FLOAT, GL_FALSE, sizeof(ExtendedBinary3DObject::Vertex), (char*)0 + 24);

		delete indices;

		return result;	
	}

	void InitializeLog() {
		ofstream * os = new ofstream();
		os->open(LOG_FILE, ios_base::out);

		if(os->good())
			Logger::getDefaultLogger()->SetOutputStream(os);
	}

}