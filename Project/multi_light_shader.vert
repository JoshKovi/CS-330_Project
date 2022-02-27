#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
	//Used to pass information to the fragment shader. 
	TexCoords = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(model) * clamp(aNormal, -1.0, 1.0); //Clamps between 1 and -1 for normals

	gl_Position = projection * view * vec4(FragPos, 1.0);
}
