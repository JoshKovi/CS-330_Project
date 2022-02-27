#include "Shader.h"

//Using the std library namespace
using namespace std;

//Class features from header are expanded here
Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	//Shader constructor takes in 2 file paths for vertex and fragment shaders

	//vertex and fragment code are used to hold string of respective shaders
	string vertexCode, fragmentCode;

	//These files are used to open their respective files and stream the information out
	ifstream vShaderFile, fShaderFile;

	//This is used to ensure the files can output exceptions, in all honesty I am relatively new
	//to enabling exceptions in this way
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);


	//Try/catch statement to read files and convert to strings
	try
	{

		//Both files are opened based off their paths
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//These stringstreams are used to hold the file data before converting to strings
		stringstream vShaderStream, fShaderStream;

		//The streams store the file data
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//files are closed, freeing up resources
		vShaderFile.close();
		fShaderFile.close();

		//The streams are converted into strings and assigned to their respective variables
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (ifstream::failure e)
	{
		//Catches errors when files are not found or cannot be accessed
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << endl;
	}

	//Converting the strings into c_strings for open gl
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//Creating the appropriate shaders vertex and fragment (IDs)
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//Attaches the appropriate shader source code to the appropriate shader
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glShaderSource(fragment, 1, &fShaderCode, NULL);

	//Compiles the Shaders
	glCompileShader(vertex);
	glCompileShader(fragment);

	//Checks for compilation errors, using enum types to ensure proper errors displayed/gl functions used
	compileErrors(vertex, VERTEX);
	compileErrors(fragment, FRAGMENT);

	//ID is initialized as a Program
	ID = glCreateProgram();

	//Shaders are attached to shader program, Shader program is linked, and checked for errors
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	compileErrors(ID, PROGRAM);


	//Shaders are deleted as they are no longer needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);




}

void Shader::compileErrors(unsigned int shader, Type type) const
{
	//Takes a ID and a type which is used to run appropriate compile/link checks
	//Success is used to ensure success compile or linking
	int success;
	//infolog stores infolog in case of failure in compile/linking
	char infoLog[512];

	//Switch Statement is used to select appropriate checks for "shader"
	switch (type) {
	case(PROGRAM):
		//Here the program is checked for successful link, if not displays message and infolog
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_FAILED::" << infoLog << endl;
		}
		break;
	default:
		//Here the shader is checked for successful compilation, if not displays message and infolog
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::" << type << infoLog << endl;
		}
		break;
	}
}

Shader::~Shader() {
	//Deletes the shader program with the ID
	glDeleteProgram(ID);
}