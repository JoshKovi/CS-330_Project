#pragma once
#include "Shapes.h"


#include <vector>
#include <glm/glm.hpp>
#include "Shapes.h"


using namespace std;


class Assets :
    public Shapes
{
public:
    struct CombinedMesh {
        //Vertex array and buffers
        GLuint VAO;
        GLuint VBOs[4];
        
        //Vector of nIndices of meshes
        vector<GLuint> nIndicesVec = {};
        
        //Vectors of vertices, indices, texCoords and normals
        vector<GLfloat> verts = {};
        vector<GLuint> indices = {};
        vector<GLfloat> texCoords = {};
        vector<GLfloat> normals = {};
        
        //vector of textureIndexs
        vector<GLuint> textureIndices = {};

        //Location, rotation, scaling
        glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 modelRot = glm::vec3(90.0f, 0.0f, 0.0f);
        glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);

        //Default Shininess 
        GLfloat shiny = 32.0f;
    };
   
    //ProtoTypes for building complex meshes
    static CombinedMesh genBullet();

    static CombinedMesh genBottle();

    static CombinedMesh genTableTop();

    static CombinedMesh genMag();

    static CombinedMesh genHammerHead();

    static CombinedMesh genHammerHandle();

    static CombinedMesh genScrewDriver();


    //Used to combine meshes and Bind textures
    static CombinedMesh CombineMesh(vector<Shapes::GLMesh> meshVec);

    static void BindTexture(Shapes::GLMesh& mesh);
};

