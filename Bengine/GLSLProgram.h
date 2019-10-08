#pragma once

#include <string>
#include <GL/glew.h>


namespace Bengine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string fragmentShaderPath, const std::string vertexShaderPath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string uniformName);

		void use();
		void unUse();

	private:

		void compileShader(const std::string filePath, GLuint id);

		int _numAttributes;

		//shader program id
		GLuint _programID;

		//shader ids
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}
