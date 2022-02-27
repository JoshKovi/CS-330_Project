#ifndef  SHADER_H
#define SHADER_H

//Required for shader class to work appropriately
#include <glad/glad.h>
#include <glm/glm.hpp>

//These will be used in the Shader.cpp file
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>


//This Header class is used to layout the Shader class
class Shader {
public:
	//Program ID, used throughout the Shader class
	unsigned int ID;

	//Constructor (reads and builds shader)
	Shader(const char* vertexPath, const char* fragmentPath);
	//Destructor, Deletes Shader Program
	~Shader();

	void use()
	{
		//Calls use program on itself
		glUseProgram(ID);
	}

	void setBool(const std::string & name, bool value) const
	{
		//Sets a Bool value for shader from uniform
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string & name, int value) const
	{
		//sets and int value for shader, from uniform
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string & name, float value) const
	{
		//Sets a float value for shader from uniform
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	//Various basic methods to set appropriate  uniforms
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	//ENUM type is easier to manage for data
	enum Type { VERTEX, FRAGMENT, PROGRAM };
	//Used to check for shader and program errors, with assistence of Type enum abovve
	void compileErrors(unsigned int shader, Type type) const;

};


#endif // ! SHADER_H