#include "Shapes.h"



void Shapes::CreateCircle(GLMesh& mesh,GLfloat x, GLfloat y, GLfloat z, GLuint Edges, GLfloat radius) {
	//This type requires triangle_fan if not indexed, TRIANGLES if indexed

	//Here the local sides vector is created, the vertices GLuint is built and double PI is assigned
	GLuint vertices = Edges + 1;
	double DbPI = 2.0f * M_PI;

	//This is the origin of the circle, where all triangles are drawn from
	mesh.sides.push_back(x);
	mesh.sides.push_back(y);
	mesh.sides.push_back(z);

	
	//For loop sets x/y based off angle from origin
	for (unsigned int i = 0; i < vertices; i++) {
		if (i + 1 == vertices) {
			mesh.sides.push_back((GLfloat)(x + (radius * std::cos(0 * DbPI / Edges)))); //X 
			mesh.sides.push_back((GLfloat)(y + (radius * std::sin(0 * DbPI / Edges)))); //Y
			mesh.sides.push_back((GLfloat)z); //Z
		}
		else {
			mesh.sides.push_back((GLfloat)(x + (radius * std::cos(i*DbPI / Edges)))); //X
			mesh.sides.push_back((GLfloat)(y + (radius * std::sin(i*DbPI / Edges)))); //Y
			mesh.sides.push_back((GLfloat)z); //Z

			//Sets indices for all vertices (last is not needed due to forward i's)
			mesh.indices.push_back(0);
			mesh.indices.push_back(i + 1);
			mesh.indices.push_back(i + 2);
		}

		
		if (i == 0) {
			//Top Point Coordinates 
			//Sets 0 to middle, sets 1 to the starting point
			mesh.texCoords.push_back(0.5f);
			mesh.texCoords.push_back(0.5f);
			mesh.texCoords.push_back((GLfloat)(0.5f + (std::cos(i * DbPI / Edges) / -2)));
			mesh.texCoords.push_back((GLfloat)(0.5f + (std::sin(i * DbPI / Edges) / 2)));
		}
		else {
			//Sets the point based off angle relational to origin
			mesh.texCoords.push_back((GLfloat)(0.5f + (std::cos(i * DbPI / Edges)/-2)));
			mesh.texCoords.push_back((GLfloat)(0.5f + (std::sin(i * DbPI / Edges)/2)));
		}
	}

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}

void Shapes::Create3DSymmetrical(GLMesh& mesh, GLfloat x, GLfloat y, GLfloat z, GLuint Edges, GLfloat radius) {
	//This only creates the outer (tube) of the object, tops and bottoms are added seperately

	//Creates two circles and holds them to draw with sides + indices
	GLMesh bottom, top;
	CreateCircle(bottom, x, y, z, Edges, radius);
	CreateCircle(top, x, y, -z, Edges, radius);
	double DbPI = 2.0f * M_PI;


	for (int i = 3; i < bottom.sides.size(); i += 3) {
		//Sets cooresponding top and bottom vertices next to each other
		mesh.sides.push_back(bottom.sides.at(i));
		mesh.sides.push_back(bottom.sides.at(i + 1));
		mesh.sides.push_back(bottom.sides.at(i + 2));

		mesh.sides.push_back(top.sides.at(i));
		mesh.sides.push_back(top.sides.at(i + 1));
		mesh.sides.push_back(top.sides.at(i + 2));

		if (i == 3) {
			//Bottom Point Coordinates 
			mesh.texCoords.push_back(0.0f);
			mesh.texCoords.push_back(1.0f);

			//Top Point Coordinates
			mesh.texCoords.push_back(0.0f);
			mesh.texCoords.push_back(0.0f);
		}
		else {
			//Bottom Coordinates
			mesh.texCoords.push_back((GLfloat)(i)/ (GLfloat)bottom.sides.size()); //X
			mesh.texCoords.push_back(1.0f); //Y
			//Top coordinates
			mesh.texCoords.push_back((GLfloat)(i) / (GLfloat)bottom.sides.size()); //X
			mesh.texCoords.push_back(0.0f); //Y
		}
	}

	for (int i = 1; i < mesh.sides.size() / 3 - 2; i++) {
		//Bottom To Top to Next Bottom
		mesh.indices.push_back(i - 1);
		mesh.indices.push_back(i);
		mesh.indices.push_back(i + 1);
		////Top to Next Bottom to Next Bottom;
		mesh.indices.push_back(i);
		mesh.indices.push_back(i + 1);
		mesh.indices.push_back(i + 2);
	}

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}

void Shapes::CreateCone(GLMesh& mesh, GLfloat x, GLfloat y,
	GLfloat z, GLuint Edges, GLfloat radius) {
	//This method is used to create a cone, I actually found out how to do this by accident while 
	//attempting to create the cylinder. Now it seems rather obviously simple, could be improved
	//by making multiple smaller circles to more gradually round the nose of the cone, will likely implement soon

	//Creates a circle vector
	CreateCircle(mesh, x, y, z, Edges, radius);


	//Puts and origin point 1.8 times the radius below z (which is above for drawing purposes) 
	mesh.sides.insert(mesh.sides.begin(), (GLfloat)(z+-radius*1.8));
	mesh.sides.insert(mesh.sides.begin(), (GLfloat)y);
	mesh.sides.insert(mesh.sides.begin(), (GLfloat)x);


	//Used for the index vector
	vector <GLuint> indices_L;
	
	//creates a index array using the offset point as the origin for each triangle
	for (unsigned int i = 1; i < (mesh.sides.size() / 3)-2; i++) {
		indices_L.push_back(0);
		indices_L.push_back(i+1);
		indices_L.push_back(i + 2);
	}
	//Assigns local indices to mesh.indices
	mesh.indices = indices_L;

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}


void Shapes::CreateSquare(GLMesh& mesh, GLfloat x, GLfloat y,
	GLfloat z, GLfloat radius) {
	
	//Creates 4 vertices, each side (rather than point) being 1 radius away from center
	mesh.sides = {
		//Right Upper
		x + radius, y + radius, z,
		//Right Lower
		x + radius, y - radius, z,
		//Left Upper
		x - radius, y + radius, z,
		//Left lower
		x - radius, y - radius, z
	};

	mesh.texCoords = {
		//Upper Right
		1.0f, 1.0f,
		//Lower Right
		1.0f, 0.0f,
		//Upper Left
		0.0f, 1.0f,
		//Lower Left
		0.0f, 0.0f
	};

	//Returns the indices of square
	mesh.indices = { 0, 1, 3, 0, 2, 3 };

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}


void Shapes::Sphere(GLMesh& mesh, GLfloat x, GLfloat y,
	GLfloat z, GLuint Edges, GLfloat radius) {

	//Needed Variables for drawing a sphere
	vector<GLuint> indices_L;
	double DbPI = M_PI * 2.0f;
	//Sets levels and latitudes to edges +1
	GLuint levels = Edges + 1;
	GLuint latitude = Edges + 1;
	


	//This for loop sets vertices based off their position relative to the origin
	for (int i = 0; i < levels; i++) {
		//This angle is the level(aka height) of the point
		GLfloat angle = i * DbPI / Edges;
		for (int j = 0; j < latitude; j++) {
			//This secAngle is the angle (x/y) from the origin
			GLfloat secAngle = j * DbPI / Edges;
			mesh.sides.push_back((GLfloat)(x - (radius * std::cos(secAngle) * std::sin(angle))));
			mesh.sides.push_back((GLfloat)(y - (radius * std::sin(secAngle) * std::sin(angle))));
			mesh.sides.push_back((GLfloat)(z - (radius * std::cos(angle))));


			//Due to the 0 index being the top point, only one point is drawn
			if (i == 0) {
				j = latitude;

				//Sets the top of half sphere to the top middle of texture
				mesh.texCoords.push_back(0.5f);
				mesh.texCoords.push_back(1.0f);
				continue;
			}

			//Due to the 0 index being the top point, only one point is drawn
			else if (i+1 == levels) {

				//Sets the top of half sphere to the top middle of texture
				mesh.texCoords.push_back(0.5f);
				mesh.texCoords.push_back(1.0f);
				continue;
			}

			//Sets roughly accurate Texture coordinates for half sphere
			mesh.texCoords.push_back(0.5f + std::cos(secAngle)/2);
			mesh.texCoords.push_back(0.5f + std::sin(angle)/2);

		}
	}

	//This for loop assigns indices, each point connects to adjacent points
	for (int i = 0; i < levels; i++) {
		for (int j = 0; j < latitude-1; j++) {
			if (i == 0) {
				//Each point in second level is connected to the top point
				indices_L.push_back(0);
				indices_L.push_back(j + 1);
				indices_L.push_back(j + 2);
			}
			else {
				//These indices connect the upper point to two lower points
				indices_L.push_back(i * Edges + j);
				indices_L.push_back(i * Edges + j + 1);
				indices_L.push_back((i-1) * Edges + j);

				//These connect the upper points to the next lower point
				indices_L.push_back((i - 1) * Edges + j);
				indices_L.push_back((i - 1) * Edges + j + 1);
				indices_L.push_back(i * Edges + j + 1);
			}
		}
	}
	//Assigns local indices to mesh indices
	mesh.indices = indices_L;

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}

void Shapes::HalfSphere(GLMesh& mesh, GLfloat x, GLfloat y,
	GLfloat z, GLuint Edges, GLfloat radius) {

	//Defines the needed variables (levels is half of edges because it is the height feature)
	vector<GLuint> indices_L;
	double PI = M_PI ;
	GLuint levels = Edges/2;
	GLuint latitude = Edges + 1;



	//This for loop sets vertices based off their position relative to the origin
	for (int i = 0; i < levels; i++) {
		//This angle is the level(aka height) of the point
		GLfloat angle = i * PI / Edges;

		//This inner For loop sets each point along a z value
		for (int j = 0; j < latitude; j++) {
			//This secAngle is the angle (x/y) from the origin
			GLfloat secAngle = j * 2*PI / Edges;
			mesh.sides.push_back((GLfloat)(x - (radius * std::cos(secAngle) * std::sin(angle))));
			mesh.sides.push_back((GLfloat)(y - (radius * std::sin(secAngle) * std::sin(angle))));
			mesh.sides.push_back((GLfloat)(z - (radius * std::cos(angle))));
			



			//Due to the 0 index being the top point, only one point is drawn
			if (i == 0) {
				j = latitude;
				
				//Sets the top of half sphere to the top middle of texture
				mesh.texCoords.push_back(0.5f);
				mesh.texCoords.push_back(1.0f);
				continue;
			}
			
			//Sets roughly accurate Texture coordinates for half sphere
			mesh.texCoords.push_back(0.5f + std::cos(secAngle)/2);
			mesh.texCoords.push_back( std::sin(angle));
		}
	}

	//This for loop assigns indices, each point connects to adjacent points
	for (int i = 0; i < levels; i++) {
		for (int j = 0; j < latitude; j++) {
			if (i == 0) {
				//Each point in second level is connected to the top point
				indices_L.push_back(0);
				indices_L.push_back(j + 1);
				indices_L.push_back(j + 2);
			}
			else {
				//These indices connect the upper point to two lower points
				indices_L.push_back(i * Edges + j);
				indices_L.push_back(i * Edges + j + 1);
				indices_L.push_back((i - 1) * Edges + j);

				//These connect the upper points to the next lower point
				indices_L.push_back((i - 1) * Edges + j);
				indices_L.push_back((i - 1) * Edges + j + 1);
				indices_L.push_back(i * Edges + j + 1);
			}
		}
	}

	//This turned out to be the easiest way to make the half sphere cut off where required
	indices_L.resize(indices_L.size() - Edges*3- 12);

	//Assigns local indices to mesh sindices
	mesh.indices = indices_L;

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}

void Shapes::RectPrism(GLMesh& mesh, GLfloat x, GLfloat y,
	GLfloat z, GLfloat radius, GLfloat height) {

	//Top and bottom meshes are created
	GLMesh top, bot;
	CreateSquare(top, x, y, z - height, radius);
	CreateSquare(bot, x, y, z, radius);

	//Both top and bottom squares are directly added to sides vector
	mesh.sides.insert(mesh.sides.end(), top.sides.begin(), top.sides.end());
	mesh.sides.insert(mesh.sides.end(), bot.sides.begin(), bot.sides.end());

	//Indices for drawing a cube
	mesh.indices = { 0, 1, 4, 4, 1, 5, 1, 3, 5, 5, 3, 7, 0, 4, 6, 6, 0, 2, 2, 3, 7, 2,6,7 };

	mesh.texCoords = {
		//Back Upper Right
		0.25f, 1.0f, 
		//Forward Upper Right
		0.75f, 1.0f,
		// Back Upper Left
		0.0f, 1.0f,
		//Forward Upper Left
		1.0f, 1.0f,
		//Back Lower Right
		0.25f, 0.0f,
		//Forward Lower Right
		0.75f, 0.0f,
		// Back Lower Left
		0.0f, 0.0f,
		//Forward Lower Left
		1.0f, 0.0f
	};

	//Sets nindices and calculates normals 
	mesh.nIndices = (GLuint)mesh.indices.size();
	mesh.normals = CalculateNormals(mesh.sides, mesh.indices, radius);
}

vector<GLfloat> Shapes::CalculateNormals(vector<GLfloat> vertices, vector<GLuint> indices, GLfloat radius) {
	//Returns a vector of normals of the same length as the vertices vec, needed due to IBO
	vector<GLfloat> normals;

	//Unordered Map allows holding of reused vertexes to average out later
	unordered_map<GLuint, vector<glm::vec3>> faceMap;
	//Used for easy tracking of highest index used
	GLuint high = 0;

	//This for loop handles sorting out the vertexs by indices
	for (int i = 0; i < indices.size(); i++) {
		//variables required for the ease of calculation
		vector<GLfloat> vertexFace;
		vector<glm::vec3> curValue;
		glm::vec3 vect1, vect2, vect3;
		glm::vec3 cross1;

		//This ensures that indices used are faces rather then adjacent points
		if ((i % 3 == 0) && (vertices.size() > indices.at(i) * 3 + 8)) {
			//vertexFace is used to hold each vertex before it is converted to a vec3
			vertexFace = { vertices.at(indices.at(i) * 3), vertices.at(indices.at(i) * 3 + 1), vertices.at(indices.at(i) * 3 + 2) };
			vect1 = glm::vec3(vertexFace.at(0), vertexFace.at(1), vertexFace.at(2));

			vertexFace = { vertices.at(indices.at(i) * 3 + 3), vertices.at(indices.at(i) * 3 + 4), vertices.at(indices.at(i) * 3 + 5) };
			vect2 = glm::vec3(vertexFace.at(0), vertexFace.at(1), vertexFace.at(2));

			vertexFace = { vertices.at(indices.at(i) * 3 + 6), vertices.at(indices.at(i) * 3 + 7), vertices.at(indices.at(i) * 3 + 8) };
			vect3 = glm::vec3(vertexFace.at(0), vertexFace.at(1), vertexFace.at(2));

			//Once all 3 vertices are vec3s, the vecs are used to determine their vectors
			//The normals are calculated with their appropriate cos, sin values to remove issues with flipping normals on sides)
			vect2 = glm::vec3(cos(vect2.x - vect1.x) / radius, sin(vect2.y - vect1.y) / radius, cos(vect2.z - vect1.z) / radius);
			vect3 = glm::vec3(cos(vect3.x - vect1.x) / radius, sin(vect3.y - vect1.y) / radius, sin(vect3.z - vect1.z) / radius);

			//The vectors are then cross multiplied with glm to get a normal vector
			cross1 = glm::cross(vect3, vect2);

			//This logic ensures each indice has this normal assigned to it
			if (faceMap.find(indices.at(i)) == faceMap.end()) {
				faceMap[indices.at(i)] = vector<glm::vec3>{ cross1 };
			}
			else {
				faceMap[indices.at(i)].push_back(cross1);
			}
			if (faceMap.find(indices.at(i + 1)) == faceMap.end()) {
				faceMap[indices.at(i + 1)] = vector<glm::vec3>{ cross1 };
			}
			else {
				faceMap[indices.at(i + 1)].push_back(cross1);
			}
			if (faceMap.find(indices.at(i + 2)) == faceMap.end()) {
				faceMap[indices.at(i + 2)] = vector<glm::vec3>{ cross1 };
			}
			else {
				faceMap[indices.at(i + 2)].push_back(cross1);
			}
		}

		//Keeps track of highest indice
		if (indices.at(i) > high) {
			high = indices.at(i);
		}
	}
	//This loop is used to build normal vector and average out vectors that arre used in multiple faces
	for (GLuint i = 0; i <= high; i++) {
		//This vec3 holds the summed value of each vertex
		glm::vec3 normalsToAvg = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < faceMap.find(i)->second.size(); j++) {
			//If this is last iteration, average the normals to Avg vector out and push to normal vector
			//due to strange lighting issue that occurs
			if (faceMap.find(i)->second.size() > 1 && j + 1 == faceMap.find(i)->second.size()) {
				normals.push_back(normalsToAvg.x / (j+1));
				normals.push_back(normalsToAvg.y / (j+1));
				normals.push_back(normalsToAvg.z / (j+1));
			}
			//Sums vertexs thatare used repeatedly
			else if (faceMap.find(i)->second.size() > 1) {
				normalsToAvg.x += faceMap.find(i)->second.at(j).x;
				normalsToAvg.y += faceMap.find(i)->second.at(j).y;
				normalsToAvg.z += faceMap.find(i)->second.at(j).z;
			}
			else {
				//Vertex is only used once and is added directly to normal vector
				normals.push_back(faceMap.find(i)->second.at(j).x);
				normals.push_back(faceMap.find(i)->second.at(j).y);
				normals.push_back(faceMap.find(i)->second.at(j).z);
			}
		}
	}

	return normals;
}