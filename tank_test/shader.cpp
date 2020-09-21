#include "shader.h"
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile(vertexPath);
	ifstream fShaderFile(fragmentPath);

	stringstream vShaderStream, fShaderStream;

	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	vShaderFile.close();
	fShaderFile.close();

	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkErrors(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool val) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

void Shader::setInt(const string& name, int val) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setFloat(const string& name, float val) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setVec3(const string& name, glm::vec3 val) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z);
}

void Shader::setVec2f(const string& name, float v1, float v2) {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
}

void Shader::setVec3f(const string& name, float v1, float v2, float v3) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setVec4f(const string& name, float v1, float v2, float v3, float v4) {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::setMat3(const string& name, glm::mat3 val) {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setMat4(const string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::checkErrors(unsigned int object, string type) {
	int success;
	char infoLog[1024];
	if (type == "VERTEX" || type == "FRAGMENT") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
	else if (type == "PROGRAM") {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
}