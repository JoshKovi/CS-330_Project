#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <corecrt_math_defines.h>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"stb_image.h"

using namespace std;

class Shapes
{
public:
	//Enum used to chose shape to render
	enum shape {CIRCLE, CYLINDER, CONE, SQUARE, SPHERE, HALF_SPHERE, TEXTURE};
	

	//This struct is built for holding important mesh information, names the VAO, both VBOs and the number of indicies (VBO[2])
	//nIndices is used to hold and draw the indexs, sides holds the vertices and modelVec is used to easily translate objects in scene
	//without having to make a bunch of complicated calls in the render loop
	struct GLMesh {
		GLuint VAO; 
		GLuint VBOs[4]; //One for Vertices, Indices, Texture Coords, Lighting
		GLuint nIndices = 0;

		//Used to store various vert attributes
		vector<GLfloat> sides = {};
		vector<GLfloat> texCoords = {};
		vector<GLuint> indices = {};
		vector<GLfloat> normals = {}; 

		//Vectors for Transformations
		glm::vec3 modelVec = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 modelRotVec = glm::vec3(90.0f, 0.0f, 0.0f); 
		glm::vec3 modelScaleVec = glm::vec3(1.0f, 1.0f, 1.0f); //Default is 1 to 1 scale

		//Holds texture indice and path
		GLuint texture; //For holding Textures
		string texturePath; //Holds Path to texture
	};

	//Prototypes of functions for drawing shapes
	static void Create3DSymmetrical(GLMesh &mesh, GLfloat x, GLfloat y,
		GLfloat z, GLuint Edges, GLfloat radius);

	static void CreateCircle(GLMesh& mesh, GLfloat x, GLfloat y, GLfloat z, GLuint Edges, GLfloat radius);

	static void CreateCone(GLMesh& mesh, GLfloat x, GLfloat y,
		GLfloat z, GLuint Edges, GLfloat radius);

	static void CreateSquare(GLMesh& mesh, GLfloat x, GLfloat y,
		GLfloat z, GLfloat radius);

	static void Sphere(GLMesh& mesh, GLfloat x, GLfloat y,
		GLfloat z, GLuint Edges, GLfloat radius);

	static void HalfSphere(GLMesh& mesh, GLfloat x, GLfloat y,
		GLfloat z, GLuint Edges, GLfloat radius);

	static void RectPrism(GLMesh& mesh, GLfloat x, GLfloat y,
		GLfloat z, GLfloat radius, GLfloat height);

	//Calculates normals for all primatives
	static vector<GLfloat> CalculateNormals(vector<GLfloat> vertices, vector<GLuint> indices, GLfloat radius);


};
#endif // !SHAPES_H
