#include "Shader.h"
namespace glutils {

	Shader::Shader(unsigned int vertexShader, unsigned int fragmentShader){
		this->init(vertexShader, 0, 0, 0, fragmentShader);
	}

	Shader::Shader(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
	{
		this->init(vertexShader, 0, 0, geometryShader, fragmentShader);
	}

	Shader::Shader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int fragmentShader) {
		this->init(vertexShader, tessControlShader, tessEvaluationShader, 0, fragmentShader);
	}

	Shader::Shader(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvaluationShader, unsigned int geometryShader, unsigned int fragmentShader) {
		this->init(vertexShader, tessControlShader, tessEvaluationShader, geometryShader, fragmentShader);
	}

	Shader::~Shader(void)
	{
		glDetachShader(this->program, this->vertex);
		glDetachShader(this->program, this->fragment);

		if(this->geometry)
			glDetachShader(this->program, this->geometry);

		if(this->tessControl)
			glDetachShader(this->program, this->tessControl);

		if(this->tessEvaluation)
			glDetachShader(this->program, this->tessEvaluation);
	}

	void Shader::init(unsigned int vertexShader, unsigned int tessControlShader, unsigned int tessEvalutationShader, unsigned int geometryShader, unsigned int fragmentShader) {
		this->program = glCreateProgram();		
		
		this->vertex = vertexShader;
		this->geometry = geometryShader;
		this->fragment = fragmentShader;
		this->tessControl = tessControlShader;
		this->tessEvaluation = tessEvalutationShader;

		glAttachShader(this->program, vertexShader);
		glAttachShader(this->program, fragmentShader);

		if(this->tessControl)
			glAttachShader(program, tessControlShader);

		if(this->tessEvaluation)
			glAttachShader(program, tessEvalutationShader);

		if(this->geometry != 0)
			glAttachShader(program, geometryShader);

		glLinkProgram(program);

		#ifdef GL_UTILS_LOG_ENABLED

			int compiled = 0;

			glGetProgramiv(program, GL_LINK_STATUS, &compiled);
			if(compiled) {
				Logger::getDefaultLogger()->writeMessage(0, "Shader::init()", "Program created and linked!");
			} else {	
				Logger::getDefaultLogger()->writeMessage(2, "Shader::init()", "Can't create the program!");
				
				int logLen;
				char * log;

				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

				log = new char[logLen];

				glGetProgramInfoLog(program, logLen, &logLen, log);
				
				Logger::getDefaultLogger()->writeMessage(2,"Shader::init()", "Program info log: " + string(log)); 
			}
		#endif
		
		glValidateProgram(program);

		#ifdef GL_UTILS_LOG_ENABLED

			int validated;

			glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);

			if(validated) {
				Logger::getDefaultLogger()->writeMessage(0, "Shader::init()", "Program validated!");
			} else {	
				Logger::getDefaultLogger()->writeMessage(2, "Shader::init()", "Can't validate the program!");
				
				int logLen;
				char * log;

				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

				log = new char[logLen];

				glGetProgramInfoLog(program, logLen, &logLen, log);
				
				Logger::getDefaultLogger()->writeMessage(2,"Shader::init()", "Program info log: " + string(log)); 
			}


		#endif

	}

	void Shader::setUniform(const char* name, int v1) {
		glUniform1i(getUniformLocation(name), v1);
	}
	void Shader::setUniform(const char* name, int v1, int v2) {
		glUniform2i(getUniformLocation(name), v1, v2);
	}
	void Shader::setUniform(const char* name, int v1, int v2, int v3) {
		glUniform3i(getUniformLocation(name), v1, v2, v3);
	}
	void Shader::setUniform(const char* name, int v1, int v2, int v3, int v4){
		glUniform4i(getUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::setUniform(const char* name, float v1) {
		glUniform1f(getUniformLocation(name), v1);
	}
	void Shader::setUniform(const char* name, float v1, float v2) {
		glUniform2f(getUniformLocation(name), v1, v2);
	}
	void Shader::setUniform(const char* name, float v1, float v2, float v3){
		glUniform3f(getUniformLocation(name), v1, v2, v3);
	}
	void Shader::setUniform(const char* name, float v1, float v2, float v3, float v4){
		glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::setUniform(const char* name, const vec2 &v) { setUniform(name, v.x, v.y); }
	void Shader::setUniform(const char* name, const vec3 &v) { setUniform(name, v.x, v.y, v.z); }
	void Shader::setUniform(const char* name, const vec4 &v) { setUniform(name, v.x, v.y, v.z, v.w); }

	void Shader::setUniform(const char* name, const ivec2 &v) { setUniform(name, v.x, v.y); }
	void Shader::setUniform(const char* name, const ivec3 &v) { setUniform(name, v.x, v.y, v.z); }
	void Shader::setUniform(const char* name, const ivec4 &v) { setUniform(name, v.x, v.y, v.z, v.w); }

	void Shader::setUniformMatrix(const char* name, mat4 &m) {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, m.elements);
	}

	void Shader::setUniformTexture(const char* name, unsigned int textureUnit, unsigned int texture) {
		setUniformTexture(name, textureUnit, texture, GL_TEXTURE_2D);
	}

	void Shader::setUniformTexture(const char* name, unsigned int textureUnit, unsigned int texture, GLenum target) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(target, texture);
		setUniform(name, (int)textureUnit);
	}

	int Shader::getUniformLocation(const char* name) {
		return glGetUniformLocation(program, name);
	}

	int Shader::getAttributeLocation(const char* name) {
		return glGetAttribLocation(program, name);
	}

	unsigned int Shader::getProgram() {
		return program;
	}

	void Shader::enable() {
		glUseProgram(program);
	}

	void Shader::disable() {
		glUseProgram(0);
	}
}

