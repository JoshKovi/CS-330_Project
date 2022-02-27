#version 440 core
out vec4 FragColor;

//Definition of structs for each light type/material

struct Material {
	//Used to determine material properties for lighting
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

//Lighting structs

struct DirLight{
	//Used for directional lights (like the sun) that come from one distant source
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	//Used for a lightsource in a position, like a lightbulb in the corner of a scene
	//(light is not going in one direction and ray angles different on each object)
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight{
	//Similar to a flashlight, with a focused directional light that diffuses quickly
	//outside of the narrow beam of light
	vec3 position;
	vec3 direction;

	float cutOff; //Light inside the beam
	float outerCutOff; //Light outside the beam but inside the diffuse (sphere of influence)

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//Inputs from Vertex Shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//Uniforms used throughout shader
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;

//function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
	//Standard used properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//Directional Lighting function (Used in the Blindss (giant white square)
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//Point lighting (bulb) which shines light in every direction
	result += CalcPointLight(pointLight, norm, FragPos, viewDir);

	//Spotlight on camera for added functionality
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

//Directional Light calculations function
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	//Used to calculate the directional light applied to a pixel
	vec3 lightDir = normalize(-light.direction);

	//diffuse, spec shading
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Finish and combine lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

//Point light calculations function
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//Used to calculate the light applied to a pixel by a point light
	vec3 lightDir = normalize(light.position - fragPos);

	//diffuse and spec
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 20.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Finish and combine lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//Applying intensity and attenuation to lighting
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

//SpotLight Calculations function
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//Used to calculate the Spotlight light value applied to a pixel
	vec3 lightDir = normalize(light.position - fragPos);
	
	//diffuse and spec
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic*(distance *distance));
	
	//Light intensity and dropoff
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


	//Finish and combine lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//Applying intensity and attenuation to lighting
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}


