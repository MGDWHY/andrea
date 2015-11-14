#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <gl\glew.h>
#include "Shader.h"
#include "Logger.h"

using namespace std;

namespace glutils {

	class ShaderManager {
	private:
		static ShaderManager *defaultManager;
		ShaderManager(void) { glewInit(); }
	public:
		static ShaderManager* getDefaultManager();
	
		unsigned int loadShaderFromFile(const string &fileName, GLenum shaderType);
		unsigned int loadShader(const string &code, GLenum shaderType);
	
		Shader* createShader(unsigned int vertexShader, unsigned int fragmentShader);
		Shader* createShader(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
		Shader* createShader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int fragmentShader);
		Shader* createShader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int geomoetryShader, unsigned int fragmentShader);

		Shader* createShader(const string vertexShaderFile, const string fragmentShaderFile);
		Shader* createShader(const string vertexShaderFile, const string geometryShaderFile, const string fragmentShaderFile);
		Shader* createShader(const string vertexShaderFile, const string tessControlShaderFile, const string tessEvaluationShaderFile, const string fragmentShaderFile);	
		Shader* createShader(const string vertexShaderFile, const string tessControlShaderFile, const string tessEvaluationShaderFile, const string geometryShaderFile, const string fragmentShaderFile);
		~ShaderManager(void);
	};
}
