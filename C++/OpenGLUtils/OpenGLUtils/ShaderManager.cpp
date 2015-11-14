#include "ShaderManager.h"

using namespace std;

namespace glutils {
	ShaderManager* ShaderManager::defaultManager;

	ShaderManager*  ShaderManager::getDefaultManager() {
		if(defaultManager == NULL)
			defaultManager = new ShaderManager;
		return defaultManager;
	}

	ShaderManager::~ShaderManager(void) {
		if(defaultManager != NULL)
			delete defaultManager;
	}

	unsigned int ShaderManager::loadShader(const string &code, GLenum shaderType) {
		unsigned int shader = glCreateShader(shaderType);
		const char **data = new const char*[1];

		data[0] = (const char*)code.c_str();

		glShaderSource(shader, 1,(const char**)data, NULL);
		glCompileShader(shader);
	
		#ifdef GL_UTILS_LOG_ENABLED
			int compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if(compiled)
				Logger::getDefaultLogger()->writeMessage(0, "ShaderManager::loadShader()", "Compiled!");
			else	
				Logger::getDefaultLogger()->writeMessage(2, "ShaderManager::loadShader()", "Not compiled!");
		#endif

		return shader;
	}

	unsigned int ShaderManager::loadShaderFromFile(const string &fileName, GLenum shaderType) {
		string source, temp;
		char **data = new char*[1];
		unsigned int shader = 0;

		ifstream in;
		in.open(fileName, ios::in);

		#ifdef GL_UTILS_LOG_ENABLED
			if(!in.good())
				Logger::getDefaultLogger()->writeMessage(2, "ShaderManager::loadShader()", "Bad file: " + fileName);
		#endif
		while(in.good()) {
			getline(in, temp);
			source += temp + "\n";
		}
	
		in.close();
	
		data[0] = (char*)source.c_str();

		#ifdef GL_UTILS_LOG_ENABLED
			Logger::getDefaultLogger()->writeMessage(0, "ShaderManager::loadShaderFromFile()", fileName + " loaded. Compiling...");
		#endif

		return loadShader(source, shaderType);
	}

	Shader* ShaderManager::createShader(unsigned int vertexShader, unsigned int fragmentShader) {
		return new Shader(vertexShader, fragmentShader);
	}
	Shader* ShaderManager::createShader(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader) {
		return new Shader(vertexShader, geometryShader, fragmentShader);
	}

	Shader* ShaderManager::createShader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int fragmentShader) {
		return new Shader(vertexShader, tessControlShader, tessEvaluationShader, fragmentShader);
	}

	Shader* ShaderManager::createShader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int geomoetryShader, unsigned int fragmentShader) {
		return new Shader(vertexShader, tessControlShader, tessEvaluationShader, geomoetryShader, fragmentShader);
	}

	Shader* ShaderManager::createShader(const string vertexShaderFile, const string tessControlShaderFile, const string tessEvaluationShaderFile, const string geometryShaderFile, const string fragmentShaderFile) {
		unsigned int vertexShader = loadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
		unsigned int tessControlShader = loadShaderFromFile(tessControlShaderFile, GL_TESS_CONTROL_SHADER);
		unsigned int tessEvaluationShader = loadShaderFromFile(tessEvaluationShaderFile, GL_TESS_EVALUATION_SHADER);
		unsigned int geometryShader = loadShaderFromFile(geometryShaderFile, GL_GEOMETRY_SHADER);
		unsigned int fragmentShader = loadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);		
		return new Shader(vertexShader, tessControlShader, tessEvaluationShader, geometryShader, fragmentShader);
	}

	Shader * ShaderManager::createShader(const string vertexShaderFile, const string tessControlShaderFile, const string tessEvaluationShaderFile, 
		const string fragmentShaderFile) {
		unsigned int vertexShader = loadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
		unsigned int tessControlShader = loadShaderFromFile(tessControlShaderFile, GL_TESS_CONTROL_SHADER);
		unsigned int tessEvaluationShader = loadShaderFromFile(tessEvaluationShaderFile, GL_TESS_EVALUATION_SHADER);
		unsigned int fragmentShader = loadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);		
		return new Shader(vertexShader, tessControlShader, tessEvaluationShader, fragmentShader);
	}

	Shader* ShaderManager::createShader(const string vertexShaderFile, const string fragmentShaderFile) {
		unsigned int vertexShader = loadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
		unsigned int fragmentShader = loadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
		return new Shader(vertexShader, fragmentShader);
	}

	Shader* ShaderManager::createShader(const string vertexShaderFile, const string geometryShaderFile, const string fragmentShaderFile) {
		unsigned int vertexShader = loadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
		unsigned int geometryShader = loadShaderFromFile(geometryShaderFile, GL_GEOMETRY_SHADER);
		unsigned int fragmentShader = loadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
		return new Shader(vertexShader, geometryShader, fragmentShader);
	}

}