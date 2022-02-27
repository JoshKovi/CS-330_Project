#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

//Shader Class and a Shapes Class
#include "Shader.h"
#include "Shapes.h"
//Camera is where most camera controls are now held
#include "Camera.h"

#include "Assets.h"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <corecrt_math_defines.h>

using namespace std;

namespace {
	//These Constants are Assigned for ease of modification
	const char* const WINDOW_TITLE = "Kovacevich_Project";
	const int WINDOW_WIDTH = 1200;
	const int WINDOW_HEIGHT = 900;

	//Used to assign viewmode, Perspective mode by default
	enum class viewMode { ORTHOGRAPHIC, PERSPECTIVE };
	viewMode viewSelect = viewMode::PERSPECTIVE;

	//Used to toggle Line mode
	enum class fillMode { FILL, LINE };
	fillMode fillSelect = fillMode::FILL;

	//Vector of combined meshes built for ease and modularity
	vector <Assets::CombinedMesh> combined = {};
	
	//Vector of light meshes
	vector<Shapes::GLMesh> lights;

	//Camera used to change view
	Camera camera(glm::vec3(0.0f, 3.0f, -10.0f));

	//Here the window is initialized as a nullptr, the mesh variable is also declared. 
	GLFWwindow* window = nullptr;

	//Used to calculate frame time, for consistent speed accross devices
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float currentFrame;

	//sets cursor position (effectively) to the middle of the screen
	float lastX = WINDOW_WIDTH / 2;
	float lastY = WINDOW_HEIGHT / 2;

	//Prevents excessive rotation when cursor first enters window
	bool firstMove = true;

	//CameraSpeed modifier, initially set to 1.0
	float cameraSpeed = 1.0f;

	//MultiDirectional light position
	glm::vec3 bulbPosition(8.0f, -10.0f, -15.0f);
	//glm::vec3 bulbPosition(1.0f, 0.16f, -4.0f);

	//Ambient Light from behind (blinds) 
	glm::vec3 blindPosition(0.0f, 8.0f, 7.0f);

	//Used to adjust directional light behind scene
	glm::vec3 lightDirAdjust(-6.1f, 31.0f, 4.2f);//(-21.4f, 15.9f, -8.1f);

	//Used to adjust colors from position light
	glm::vec3 lightColor(0.25f, 0.25f, 0.15f);

	//Used to count triangles
	bool TrianglesCounted = false;
	int TrianglesDrawn = 0;
}

//These are the prototypes for the functions to be used
bool InitializeOpenGL(GLFWwindow**); //
void ResizeWindow(GLFWwindow*, int, int); //
void ProcessInput(GLFWwindow*); //

Shapes::GLMesh CreateMesh(GLfloat x, GLfloat y, GLfloat z, GLuint Edges, GLfloat radius, Shapes::shape);
void DestroyMesh(vector<Shapes::GLMesh> meshes);
void DestroyMesh(vector<Assets::CombinedMesh> meshes);

void Render(Shader& shader, Shader& lightShader);

//New protoypes
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void speedAdjustment(double yOffset);



int main() {

	//Ensures window was properly initialized
	if (!InitializeOpenGL(&window)) {
		std::cout << "Glad failed to Initialize Correctly" << std::endl;
		return -1;
	}

	//Generates bottle mesh with all included primatives and textures
	combined.push_back(Assets::genBottle());
	combined.at(0).modelPos = glm::vec3(3.0f, 3.01f, 3.0f);
	combined.at(0).modelRot.z = -45.0f;
	combined.at(0).shiny = 16.0f;

	//Generates the bullet mesh with all included primatives and textures
	combined.push_back(Assets::genBullet());
	combined.at(1).modelPos = glm::vec3(0.0f, 0.155f, -3.5f);
	combined.at(1).shiny = 32.0f;

	//Copies and adjusts original bullet 
	combined.push_back(combined.at(1));
	combined.at(2).modelPos = glm::vec3(0.0f, 0.1f, -3.95f);
	combined.at(2).modelRot.x = 0.0f;
	combined.at(2).modelRot.y = 45.0f;
	
	//Copies and adjusts original bullet 
	combined.push_back(combined.at(1));
	combined.at(3).modelPos = glm::vec3(-0.25f, 0.18f, -3.5f);
	combined.at(3).modelRot.x = 135.0f;

	//Generates the TableTop mesh
	combined.push_back(Assets::genTableTop());
	combined.at(4).shiny = 16.0f;

	//Generates the mesh with all primatives and textures for magazine
	combined.push_back(Assets::genMag());
	combined.at(5).modelPos = glm::vec3(-1.6f, 0.23f, -3.1f);
	combined.at(5).modelRot = glm::vec3(-120.0f, -87.0f, -30.0f);
	combined.at(5).shiny = 48.0f;

	//Copies bullet and positions relative to magazine (gives impression of loaded magazine
	combined.push_back(combined.at(1));
	combined.at(6).modelPos = glm::vec3(combined.at(5).modelPos.x+2, combined.at(5).modelPos.y/2+0.03f, combined.at(5).modelPos.z+0.14f);
	combined.at(6).modelRot.x = 0.0f;

	//Generates hammerhead (only uses one texture) 
	//Note: I attempted to make a claw hammer for hours but the geometry involved with the claw portion proved to difficult
	//To generate manually, as such I generated a simple metal deadblow mallet instead
	combined.push_back(Assets::genHammerHead());
	combined.at(7).modelPos = glm::vec3(-1.6f, 0.31f, 1.1f);
	combined.at(7).modelRot = glm::vec3(0.0f, -45.0f, 0.0f);

	//Handle for the hammer, positioned relative to hammer head
	combined.push_back(Assets::genHammerHandle());
	combined.at(8).modelRot = glm::vec3(0.0f, 45.0f, -80.0f);
	combined.at(8).modelPos = glm::vec3(combined.at(7).modelPos.x-1.6f, combined.at(7).modelPos.y-0.03f, combined.at(7).modelPos.z-1.6f);
	combined.at(8).shiny = 16.0f;

	//Generates screwdrive and positions
	combined.push_back(Assets::genScrewDriver());
	combined.at(9).modelPos = glm::vec3(-3.0f, 0.61f, -1.1f);
	combined.at(9).modelRot = glm::vec3(-120.0f, -73.5f, -70.0f);


	//For loop binds each texture used, I have no issues (RTX 2080) here but I suspect a computer
	//with an older graphics card or only 16 Textures slots may have an issue, which would require
	//splitting the generation to multiple vectors and binding each texture prior to drawing
	GLuint lastGenedText = 0;
	for (int i = 0; i < combined.size(); i++) {
		for (int j = 0; j < combined.at(i).textureIndices.size(); j++) {
			if (combined.at(i).textureIndices.at(j) >= lastGenedText) {
				glActiveTexture(GL_TEXTURE0 + combined.at(i).textureIndices.at(j));
				glBindTexture(GL_TEXTURE_2D, combined.at(i).textureIndices.at(j));
				lastGenedText = combined.at(i).textureIndices.at(j);
			}
		}
	}

	//Lighting meshes and shader-------------------------------------------------------
	//Creates all empty meshes and adds them to the meshes vector 
	for (int i = 0; i < 2; i++) {
		Shapes::GLMesh mesh;
		lights.push_back(mesh);
	}

	//Sphere (light bulb) roughly positioned where intended
	lights.at(0)= CreateMesh(bulbPosition.x, bulbPosition.y, bulbPosition.z, 20, 1.0f, Shapes::SPHERE);

	//Window Blinds
	lights.at(1) = CreateMesh(blindPosition.x, blindPosition.y, blindPosition.z, 4, 5.0f, Shapes::SQUARE);
	lights.at(1).modelRotVec.x = 0.0f;

	//This is a bassic shader, as the light objects are just a representation and are not textured
	//nor do they react to other light sources
	Shader lightShader("lightShader.vert", "lightShader.frag");
	//Lighting meshes and shader end-------------------------------------------------------


	//Calls create mesh, creates a shader, uses that shader and sets GL_DEPTH_TEST to enabled to ensure Z axis is properly used.
	//This is the new version of both the object vert and frag shaders
	Shader shader("multi_light_shader.vert", "multi_light_shader.frag");
	shader.use();

	glEnable(GL_DEPTH_TEST);

	//Main Loop
	while (!glfwWindowShouldClose(window)) {
		//Used to track frames and equilize movement
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//Processes input, sets clear color, then clears Color and Depth buffers
		ProcessInput(window);
		glClearColor(0.3f, 0.4f, 0.2f, 0.8f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calls render method
		Render(shader, lightShader);

		//Swaps buffers and polls events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Properly disposes of assets before closing
	shader.~Shader();
	glfwTerminate();
	DestroyMesh(lights);
	DestroyMesh(combined);

	return 0;
}





void Render(Shader& shader, Shader& lightShader) {

	shader.use();

	//This sets Camera position 
	shader.setVec3("viewPos", camera.Position);
	

	//This is the light behind the scene (the blinds portion of my scene) all light comes from behind
	//direction can be move x with left and right, y with up and down, z with m and n
	shader.setVec3("dirLight.direction", lightDirAdjust);
	shader.setVec3("dirLight.ambient", 0.45f, 0.45f, 0.45f);
	shader.setVec3("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
	shader.setVec3("dirLight.specular", 0.6f, 0.6f, 0.6f);

	//This is the bulb light, the range has been drastically extended in the shader
	shader.setVec3("pointLight.position", bulbPosition);
	shader.setVec3("pointLight.ambient", lightColor);
	shader.setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader.setVec3("pointLight.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("pointLight.constant", 0.25f);
	shader.setFloat("pointLight.linear", 0.09f);
	shader.setFloat("pointLight.quadratic", 0.032f);

	//This is the camera connected spotlight
	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09f);
	shader.setFloat("spotLight.quadratic", 0.032f);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	//Declars projection
	glm::mat4 projection;

	//Sets line mode for shapes, allowing easy troubleshooting or views (L key)
	if (fillSelect == fillMode::LINE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (fillSelect == fillMode::FILL) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Sets the projection mode (between perspective and orthographic)(P key)
	if (viewSelect == viewMode::PERSPECTIVE) {
		projection = glm::perspective(glm::radians(camera.zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
	if (viewSelect == viewMode::ORTHOGRAPHIC) {
		projection = glm::ortho(-5.0f, 5.0f, -1.0f, 5.0f, 0.01f, 100.0f);//Size due to plane/model size
	}
	shader.setMat4("projection", projection);

	//Creates required glm Mat4 objects, assigns view
	glm::mat4 model;
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", view);

	for (int i = 0; i < combined.size(); i++) {
		glBindVertexArray(combined.at(i).VAO);

		//Translates model to appropriate position after scaling as needed
		model = glm::mat4(1.0f);
		model = glm::scale(model, combined.at(i).modelScale);
		model = glm::translate(model, combined.at(i).modelPos);

		//Uses a vec 3 holding degrees to assign proper rotation of each axis
		model = glm::rotate(model, glm::radians(combined.at(i).modelRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(combined.at(i).modelRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(combined.at(i).modelRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMat4("model", model);

		for (int j = 0; j < combined.at(i).textureIndices.size(); j++) {
			//Sets the appropriate texture and then progressives draws indices with appropriate texture
			//As each texture requires its own draw call (as far as I can tell) in opengl
			shader.setFloat("material.shininess", combined.at(i).shiny);
			shader.setInt("material.diffuse", combined.at(i).textureIndices.at(j));
			shader.setInt("material.specular", combined.at(i).textureIndices.at(j));
			glDrawElements(GL_TRIANGLES, combined.at(i).nIndicesVec.at(j), GL_UNSIGNED_INT, (void*)0);

			if (!TrianglesCounted) {
				TrianglesDrawn += combined.at(i).nIndicesVec.at(j)/3;
				if (j > 0) {
					TrianglesDrawn -= combined.at(i).nIndicesVec.at(j - 1) / 3;
				}
			}

		}

		glBindVertexArray(0);
	}

	if (!TrianglesCounted) {
		cout << "Total Triangle Count for all images (excluding lights): " << TrianglesDrawn << endl;
		TrianglesCounted = true;
	}


	//Light rendering-----------------------------------------------------------
	//This section is used to draw the lighting elements, rotation was used for the plan
	//as it was easier this way
	lightShader.use();
	lightShader.setMat4("projection", projection);
	lightShader.setMat4("view", view);
	for (unsigned int i = 0; i < lights.size(); i++) {
		glBindVertexArray(lights.at(i).VAO);
		lightShader.setVec3("boxColor", glm::vec3(1.0f));
		model = glm::mat4(1.0f);
		//Uses a vec 3 holding degrees to assign proper rotation of each axis
		model = glm::rotate(model, glm::radians(lights.at(i).modelRotVec.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(lights.at(i).modelRotVec.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(lights.at(i).modelRotVec.z), glm::vec3(0.0f, 0.0f, 1.0f));
		lightShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, lights.at(i).nIndices, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
}


Shapes::GLMesh CreateMesh(GLfloat x, GLfloat y, GLfloat z, GLuint Edges, GLfloat radius, Shapes::shape shapeType) {
	//Only used to gen Lights now as other shapes creation was moved to Assets class
	
	//Temp mesh for creating and returning shapes
	Shapes::GLMesh mesh;

	//indices is used to hold vertex indexs for EBO
	vector<GLuint> indices;

	//Calls appropriate shape creation method with use of enum
	switch (shapeType) {
	case Shapes::CYLINDER:
		Shapes::Create3DSymmetrical(mesh, x, y, z, Edges, radius);
		break;
	case Shapes::CIRCLE:
		Shapes::CreateCircle(mesh, x, y, z, Edges, radius); //Refactored for texture purposes
		break;
	case Shapes::CONE:
		Shapes::CreateCone(mesh, x, y, z, Edges, radius);
		break;
	case Shapes::SQUARE:
		Shapes::CreateSquare(mesh, x, y, z, radius);
		break;
	case Shapes::SPHERE:
		Shapes::Sphere(mesh, x, y, z, Edges, radius);
		break;
	case Shapes::HALF_SPHERE:
		Shapes::HalfSphere(mesh, x, y, z, Edges, radius);
		break;
	case Shapes::TEXTURE://To be used in combining meshes, code removed for this assignment (still work in progress for textures)
		break;

	}

	//Assigns mesh indices size to nIndices for element drawing/use
	mesh.nIndices = (GLuint)mesh.indices.size();

	//Creates a VAO to hold buffer objects
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);

	//Creates 2 buffer objects, one for vertices and indices
	glGenBuffers(2, mesh.VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.sides.size(), &mesh.sides[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.VBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);

	return mesh;
}

void DestroyMesh(vector<Shapes::GLMesh> meshes) {
	//Destroys the meshs Vertex Array Object and Vertex Buffer Objects
	for (int i = 0; i < meshes.size(); i++) {
		glDeleteVertexArrays(1, &meshes.at(i).VAO);
		glDeleteBuffers(2, meshes.at(i).VBOs);
	}

}

void DestroyMesh(vector<Assets::CombinedMesh> meshes) {
	//Destroys the Combined meshs Vertex Array Object and Vertex Buffer Objects
	for (int i = 0; i < meshes.size(); i++) {
		glDeleteVertexArrays(1, &meshes.at(i).VAO);
		glDeleteBuffers(4, meshes.at(i).VBOs);
	}
}

bool InitializeOpenGL(GLFWwindow** window) {
	//These lines initialize glfw, give the version (4.4 in this case) and specifies use of the OpenGL Core Profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Enables OpenGL on Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Creates window with preassigned settings (above) then ensures the window was appropriately created
	* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL) {
		//Terminates if window creation fails
		std::cout << "Window creation failed." << std::endl;
		glfwTerminate();
		return false;
	}

	//Sets the current context to the created window and sets a resize function
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, ResizeWindow);

	//These three functions disable the mouse, then set the Cursor and scrollwheel movement callback functions
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(*window, mouse_callback);
	glfwSetScrollCallback(*window, scroll_callback);

	//Loads glad and ensures Glad is working correctly
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}

	return true;
}


void ResizeWindow(GLFWwindow* window, int width, int height) {
	//tells OpenGL the size of the window, first 2 parameters tell lower left corner position, next 2 width and height (in pixels)
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {

	//This is used to determine the speed modifier of the camera movement
	deltaTime *= cameraSpeed;
	//Holding Shift increases speed drastically, be careful
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		deltaTime *= 20.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//Camera Movement
	//Each key below is used with some camera movement direction through the use of an Enum
	//The camera function is called to adjust the viewpoint of the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::Camera_Movement::DOWN, deltaTime);
	}

	//This toggles between Perspective and orthographic mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		//Creates a toggle between view modes used in renderer
		switch (viewSelect) {
		case(viewMode::PERSPECTIVE):
			viewSelect = viewMode::ORTHOGRAPHIC;
			break;
		case(viewMode::ORTHOGRAPHIC):
			viewSelect = viewMode::PERSPECTIVE;
			break;
		}
	}
	//This toggles between Perspective and orthographic mode
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		//Creates a toggle between view modes used in renderer
		switch (fillSelect) {
		case(fillMode::FILL):
			fillSelect = fillMode::LINE;
			break;
		case(fillMode::LINE):
			fillSelect = fillMode::FILL;
			break;
		}
	}

	//Adjusting the lighting direction (for window blinds), simulates the outside light shining through differently
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		lightDirAdjust.x += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		lightDirAdjust.x -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		lightDirAdjust.y -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		lightDirAdjust.y += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		lightDirAdjust.z -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		lightDirAdjust.z += 0.1f;
		//cout << "X: " << lightDirAdjust.x << ", Y: " << lightDirAdjust.y << ", Z: " << lightDirAdjust.z << endl; //uncomment to see values
	}

	//Adjusting the lighting color (ambient) (for bulb), RGB for increases, THN Decreases respectfully
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if(lightColor.x <=1.0f){
			lightColor.x += 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (lightColor.x >= 0.0f) {
			lightColor.x -= 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (lightColor.y >= 0.0f) {
			lightColor.y -= 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (lightColor.y <= 1.0f) {
			lightColor.y += 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (lightColor.z >= 0.0f) {
			lightColor.z -= 0.005f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (lightColor.z <= 1.0f) {
			lightColor.z += 0.005f;
		}
		//cout << "X: " << lightColor.x << ", Y: " << lightColor.y << ", Z: " << lightColor.z << endl; //uncomment to see values
	}

	//F resets adjustable light attributes to default
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		lightColor = glm::vec3(0.25f, 0.25f, 0.15f);
		lightDirAdjust = glm::vec3(-6.1f, 31.0f, 4.2f);
	}


}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

	//Assigns the x and y positions of the cursor to x and y Pos
	float xPos = static_cast<float>(xposIn);
	float yPos = static_cast<float>(yposIn);

	//Prevents wild movements when mouse enters the screen
	if (firstMove) {
		lastX = (float)xposIn;
		lastY = (float)yposIn;
		firstMove = false;
	}

	//sets x and y offsets based off current and last cursor position
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;

	//Sets the current position to the last position for later use
	lastX = xPos;
	lastY = yPos;

	//Calls camera method to adjust view vectors
	camera.ProcessMouseInput(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//Simply calls speedAdjustment with yoffset to adjust camera speed modifier
	speedAdjustment(yoffset);
	
	//Not used as scroll is to control speed per rubric
	//camera.ProcessScroll(static_cast<float>(yoffset));
}

void speedAdjustment(double yOffset) {

	//Forward scrolls increase speed, aftward scrolls reduce speed
	cameraSpeed += (float)yOffset * 5.0f;

	//Speed is capped between 1 and 25 to avoid excessive speed/slow movement
	if (cameraSpeed < 1.0f) {
		cameraSpeed = 1.0;
	}
	if (cameraSpeed > 25.0f) {
		cameraSpeed = 25.0;
	}
}

