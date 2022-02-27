#include "Assets.h"


Assets::CombinedMesh Assets::genBullet() {
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 4; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Casing of bullet
	Shapes::Create3DSymmetrical(tempMeshes.at(0), 0.0f, 0.0f, 0.15f, 20, 0.1f);
	tempMeshes.at(0).texturePath = "./Casing.jpg";

	//Top of bullet
	Shapes::CreateCircle(tempMeshes.at(1), 0.0f, 0.0f, 0.15f, 20, 0.1f);
	tempMeshes.at(1).texturePath = "./CaseBot.jpg"; //just the rim shows

	//Bottom of bullet (Primer and specification)
	Shapes::CreateCircle(tempMeshes.at(2), 0.0f, 0.0f, -0.15f, 20, 0.1f);
	tempMeshes.at(2).texturePath = "./CaseBot.jpg";

	//Bullet
	Shapes::HalfSphere(tempMeshes.at(3), 0.0f, 0.0f, 0.3f, 24, 0.1f);
	tempMeshes.at(3).texturePath = "./Bullet.jpg";
	tempMeshes.at(3).modelVec.z = -0.715f;
	tempMeshes.at(3).modelScaleVec.z = 2.0f; //Makes the half Sphere as Tall as it is wide(looks like a bullet)


	return CombineMesh(tempMeshes);

}

Assets::CombinedMesh Assets::genBottle() {
	//Generates required mesh vector for usage
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 5; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Creates the required primatives and scales/translates to appropriate location within overall mesh
	//Large Cylinder AKA Water Bottle
	Shapes::Create3DSymmetrical(tempMeshes.at(0), 0.0f, 0.0f, 3.0f, 20, 1.25f);
	tempMeshes.at(0).texturePath = "./Bottle.jpg";

	////Bottom of water bottom
	Shapes::CreateCircle(tempMeshes.at(1), 0.0f, 0.0f, 3.0f, 20, 1.25f);
	tempMeshes.at(1).texturePath = "./BottleBot.jpg";

	//Top of water bottle
	Shapes::CreateCircle(tempMeshes.at(2), 0.0f, 0.0f, -3.0f, 20, 1.25f);
	tempMeshes.at(2).texturePath = "./BottleTop.jpg";

	//The sides of the lid
	Shapes::Create3DSymmetrical(tempMeshes.at(3), 0.0f, 0.0f, 0.25f, 20, 1.0f);
	tempMeshes.at(3).texturePath = "./LidSide.jpg";
	tempMeshes.at(3).modelVec.z = -3.26f;

	//Only the Top of the cylinder needs rendered/textured
	Shapes::CreateCircle(tempMeshes.at(4), 0.0f, 0.0f, 0.25f, 20, 1.0f);
	tempMeshes.at(4).texturePath = "./LidTop.jpg";
	tempMeshes.at(4).modelVec.z = -3.76f;

	return CombineMesh(tempMeshes);

}

Assets::CombinedMesh Assets::genTableTop() {
	//Creates a mesh vector with only the tableTop mesh and "combines" it into combinedMesh
	vector<Shapes::GLMesh> tempMeshes;
	Shapes::GLMesh mesh;
	Shapes::CreateSquare(mesh, 0.0f, 0.0f, 0.0f, 5.0f);
	mesh.texturePath = "./TableTop.jpg";
	mesh.modelVec = glm::vec3(0.0f, 0.0f, 0.0f);
	tempMeshes.push_back(mesh);

	return CombineMesh(tempMeshes);
}

Assets::CombinedMesh Assets::genMag() {
	//Generates required mesh vector for usage
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 5; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Creates the required primatives and scales/translates to appropriate location within overall mesh
	//Magazine Grip portion
	Shapes::Create3DSymmetrical(tempMeshes.at(0), 0.0f, 0.0f, 0.15f, 20, 0.1f);
	tempMeshes.at(0).modelScaleVec = glm::vec3(2.0f, 3.75f, 2.0f);
	tempMeshes.at(0).texturePath = "./MagHanFront.jpg";

	//Bottom of magazing grip
	Shapes::CreateCircle(tempMeshes.at(1), 0.0f, 0.0f, 0.15f, 20, 0.1f);
	tempMeshes.at(1).modelScaleVec = glm::vec3(2.0f, 3.75f, 2.0f);
	tempMeshes.at(1).texturePath = "./MagBot.jpg"; 

	//Top of magazine grip
	Shapes::CreateCircle(tempMeshes.at(2), 0.0f, 0.0f, -0.15f, 20, 0.1f);
	tempMeshes.at(2).modelScaleVec = glm::vec3(2.0f, 3.75f, 2.0f);
	tempMeshes.at(2).texturePath = "./MagBot.jpg"; //just the rim shows

	//Magazine (metal piece)
	Shapes::RectPrism(tempMeshes.at(3), 0.0f, 0.0f, 0.0f, 0.25f, 2.0f);
	tempMeshes.at(3).modelScaleVec = glm::vec3(0.5f, 1.0f, 1.0f);
	tempMeshes.at(3).texturePath = "./MagMet.jpg";

	//Magazine top
	Shapes::CreateSquare(tempMeshes.at(4), 0.0, 0.0, -2.0f, 0.25f);
	tempMeshes.at(4).modelScaleVec = glm::vec3(0.5f, 1.0f, 1.0f);
	tempMeshes.at(4).texturePath = "./MagTop.jpg";
	
	return CombineMesh(tempMeshes);
}

Assets::CombinedMesh Assets::genHammerHead() {
	//Generates required mesh vector for usage
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 3; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Creates the required primatives and scales/translates to appropriate location within overall mesh
	//Hammer head (deadblow rather than claw style)
	Shapes::Create3DSymmetrical(tempMeshes.at(0), 0.0f, 0.0f, 0.75f, 16, 0.15f);
	tempMeshes.at(0).modelScaleVec = glm::vec3(2.0f, 2.0f, 1.0f);
	tempMeshes.at(0).texturePath = "./HamHead.jpg";

	//Hammer head cap
	Shapes::CreateCircle(tempMeshes.at(1), 0.0f, 0.0f, 0.75f, 16, 0.15f);
	tempMeshes.at(1).modelScaleVec = glm::vec3(2.0f, 2.0f, 1.0f);
	tempMeshes.at(1).texturePath = "./HamHead.jpg";

	//Hammer head cap
	Shapes::CreateCircle(tempMeshes.at(2), 0.0f, 0.0f, -0.75f, 16, 0.15f);
	tempMeshes.at(2).modelScaleVec = glm::vec3(2.0f, 2.0f, 1.0f);
	tempMeshes.at(2).texturePath = "./HamHead.jpg";


	return CombineMesh(tempMeshes);
}

Assets::CombinedMesh Assets::genHammerHandle() {
	//Generates required mesh vector for usage
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 3; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Creates the required primatives and scales/translates to appropriate location within overall mesh
	//Hammer Handle
	Shapes::Create3DSymmetrical(tempMeshes.at(0), 0.0f, 0.0f, 0.75f, 16, 0.15f);
	tempMeshes.at(0).modelScaleVec = glm::vec3(1.75f, 2.0f, 3.0f);
	tempMeshes.at(0).texturePath = "./HamHandle.jpg";

	//Hammer Top Cap
	Shapes::CreateCircle(tempMeshes.at(1), 0.0f, 0.0f, 2.25f, 16, 0.15f);
	tempMeshes.at(1).modelScaleVec = glm::vec3(1.75f, 2.0f, 1.0f);
	tempMeshes.at(1).texturePath = "./LidTop.jpg";

	//Hammer Bot Cap
	Shapes::CreateCircle(tempMeshes.at(2), 0.0f, 0.0f, -2.25f, 16, 0.15f);
	tempMeshes.at(2).modelScaleVec = glm::vec3(1.75f, 2.0f, 1.0f);
	tempMeshes.at(2).texturePath = "./LidTop.jpg";

	return CombineMesh(tempMeshes);
}

Assets::CombinedMesh Assets::genScrewDriver() {
	//Generates required mesh vector for usage
	vector<Shapes::GLMesh> tempMeshes;
	for (int i = 0; i < 4; i++) {
		Shapes::GLMesh mesh;
		tempMeshes.push_back(mesh);
	}

	//Creates the required primatives and scales/translates to appropriate location within overall mesh
	//First half of handle
	Shapes::HalfSphere(tempMeshes.at(0), 0.0f, 0.0f, 0.0f, 20, 0.15f);
	tempMeshes.at(0).modelScaleVec = glm::vec3(1.75f, 2.0f, 6.0f);
	tempMeshes.at(0).modelVec = glm::vec3(0.0f, 0.0f, 0.3f);
	tempMeshes.at(0).texturePath = "./ScrewHandle.jpg";

	//Second hald of handle
	Shapes::HalfSphere(tempMeshes.at(1), 0.0f, 0.0f, 0.0f, 20, 0.15f);
	tempMeshes.at(1).modelScaleVec = glm::vec3(1.75f, 2.0f, -6.0f);
	tempMeshes.at(1).texturePath = "./ScrewHandle.jpg";

	//Screwdriver shaft
	Shapes::Create3DSymmetrical(tempMeshes.at(2), 0.0f, 0.0f, 1.0f, 16, 0.05f);
	tempMeshes.at(2).modelScaleVec = glm::vec3(1.0f, 1.0f, 1.0f);
	tempMeshes.at(2).modelVec = glm::vec3(0.0f, 0.0f, -1.25f);
	tempMeshes.at(2).texturePath = "./ScrewShaft.jpg";

	//Screwdriver head
	Shapes::HalfSphere(tempMeshes.at(3), 0.0f, 0.0f, 0.0f, 20, 0.05f);
	tempMeshes.at(3).modelScaleVec = glm::vec3(1.0f, 1.0f, 2.0f);
	tempMeshes.at(3).modelVec = glm::vec3(0.0f, 0.0f, -2.23f);
	tempMeshes.at(3).texturePath = "./ScrewHead.jpg";


	return CombineMesh(tempMeshes);
}


Assets::CombinedMesh Assets::CombineMesh(vector<Shapes::GLMesh> meshVec) {
	//Declares a Combined mesh and sets highest index to 0
	CombinedMesh combinedMesh;
	GLuint lastHighestIndex = 0;

	//For loop used to combine meshes
	for (int i = 0; i < meshVec.size(); i++) {

		//Performs translations to vertices and adds to combined meshes verts
		for (int j = 0; j < meshVec.at(i).sides.size(); j += 3) {
			combinedMesh.verts.push_back((meshVec.at(i).sides.at(j) * meshVec.at(i).modelScaleVec.x) + meshVec.at(i).modelVec.x);
			combinedMesh.verts.push_back((meshVec.at(i).sides.at(j + 1) * meshVec.at(i).modelScaleVec.y) + meshVec.at(i).modelVec.y);
			combinedMesh.verts.push_back((meshVec.at(i).sides.at(j + 2) * meshVec.at(i).modelScaleVec.z) + meshVec.at(i).modelVec.z);
		}

		//Adds normals and texture coordinates to combined mesh
		combinedMesh.normals.insert(combinedMesh.normals.end(), meshVec.at(i).normals.begin(), meshVec.at(i).normals.end());
		combinedMesh.texCoords.insert(combinedMesh.texCoords.end(), meshVec.at(i).texCoords.begin(), meshVec.at(i).texCoords.end());

		//Binds a texture if not already done so/assigned.
		if (!meshVec.at(i).texturePath.empty()) {
			BindTexture(meshVec.at(i));
		}
		
		//Adds texture indice to combined mesh
		combinedMesh.textureIndices.push_back(meshVec.at(i).texture);
		
		//adds indices location to n indices vector
		if (i > 0) {
			combinedMesh.nIndicesVec.push_back(meshVec.at(i).nIndices + combinedMesh.nIndicesVec.at(i - 1));
		}
		else {
			combinedMesh.nIndicesVec.push_back(meshVec.at(i).nIndices);
		}


		//adds indices (with corrected indexs) 
		for (int j = 0; j < meshVec.at(i).nIndices; j++) {
			combinedMesh.indices.push_back(meshVec.at(i).indices.at(j) + lastHighestIndex + i);
		}
		//Adds highest index to the last highest index
		lastHighestIndex += meshVec.at(i).indices.at(meshVec.at(i).indices.size() - 1);

	}

	//Creates a VAO to hold buffer objects
	glGenVertexArrays(1, &combinedMesh.VAO);
	glBindVertexArray(combinedMesh.VAO);

	//Creates 4 buffer objects, one for vertices, indices, texture coordinates, and normals
	glGenBuffers(4, combinedMesh.VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, combinedMesh.VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combinedMesh.verts.size(), &combinedMesh.verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, combinedMesh.VBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * combinedMesh.indices.size(), &combinedMesh.indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);

	//Textures 
	glBindBuffer(GL_ARRAY_BUFFER, combinedMesh.VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combinedMesh.texCoords.size(), &combinedMesh.texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(2);

	//Add Normals in 4th buffer
	glBindBuffer(GL_ARRAY_BUFFER, combinedMesh.VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combinedMesh.normals.size(), &combinedMesh.normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	
	return combinedMesh;
}

void Assets::BindTexture(Shapes::GLMesh& mesh) {
	//Binds the Mesh's VAO and binds the unused 3rd VBO for texture Coords
	glBindVertexArray(mesh.VAO);

	//Gens and Binds 2D texture
	glGenTextures(1, &mesh.texture);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	//Sets GL_Texture_2D parameters as required
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//LOAD AND GEN TEXTURE
	//Texture Related size variables
	int width, height, nrChannels;
	//Sets path to the Texture path
	const char* path = mesh.texturePath.c_str();
	//Loads texture image and sets width, height and channels (in function)
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	//cout << width << ", " << height << ", " << nrChannels; //Only works with sizes that are multiples of 8
	if (data) {
		//If data was successfully created, generate the Minmap and image texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	//Frees the Image data from memory
	stbi_image_free(data);
}

