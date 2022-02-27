#version 440 core

out vec4 FragColor;

uniform vec3 boxColor;

void main(){
	//Assigns bright white color for each light element
	FragColor = vec4(boxColor, 1.0);
}