#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

namespace Bengine {

	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{

	}

	GLSLProgram::~GLSLProgram() {

	}

	//compile vertex and fragment shaders
	void GLSLProgram::compileShaders(const std::string vertexShaderPath, const std::string fragmentShaderPath) {

		// Get a program object.
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		if (_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		if (_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to be created");
		}

		//compile shaders
		compileShader(vertexShaderPath, _vertexShaderID);
		compileShader(fragmentShaderPath, _fragmentShaderID);
	}

	//links vertex and fragment shaders
	void GLSLProgram::linkShaders() {

		// Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		// Link our program
		glLinkProgram(_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_programID);

			// Delete shaders
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);


			//proivide the error log
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaderss failed to link!");
		}

		// Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);

		//delete shaders to return resources
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

	}

	//binds attributes to program
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	//gets uniform location for fragment shader
	GLint GLSLProgram::getUniformLocation(const std::string uniformName) {
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}

		return location;
	}


	//uses created shader program
	void GLSLProgram::use() {
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	//closes created shader program
	void GLSLProgram::unUse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	//code to compile shaders
	void GLSLProgram::compileShader(const std::string filePath, GLuint id) {

		//gets shader code from shader file
		std::ifstream file(filePath);

		//checking if file failed to open
		if (file.fail()) {
			fatalError("Failed to open" + filePath);
		}

		//strings to store file info
		std::string fileContents = "";
		std::string line;

		//getting info from file
		while (std::getline(file, line)) {
			fileContents += line + "\n";
		}

		//close file once used
		file.close();

		//pointer for creating shader source code
		const char* contentsPtr = fileContents.c_str();

		//creates shader source code
		glShaderSource(id, 1, &contentsPtr, nullptr);

		//compiles shader source code
		glCompileShader(id);

		//checking for errors in compiling shader
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//the maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			//proivide the error log
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");

			//exit with failure
			glDeleteShader(id);
		}

	}
}