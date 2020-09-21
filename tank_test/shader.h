#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const string& name, bool val);
	void setInt(const string& name, int val);
	void setFloat(const string& name, float val);
	void setVec3(const string& name, glm::vec3 val);
	void setVec2f(const string& name, float v1, float v2);
	void setVec3f(const string& name, float v1, float v2, float v3);
	void setVec4f(const string& name, float v1, float v2, float v3, float v4);
	void setMat3(const string& name, glm::mat3 val);
	void setMat4(const string& name, glm::mat4 val);

private:
	void checkErrors(unsigned int object, string type);
};

#endif