#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;
uniform Material material;

void main()
{    
    vec3 ambient = material.ambient;
    vec3 diffuse = material.diffuse;
    vec3 specular = material.specular;
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}