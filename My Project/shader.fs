#version 330 core

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);
	// FragColor = texture(material.diffuseMap, TexCoords);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	// vec3 ambient = light.ambient * material.ambient;
	// vec3 diffuse = diff * light.diffuse * material.diffuse;
	// vec3 specular = spec * light.specular * material.specular;

	vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoords));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuseMap, TexCoords));
	vec3 specular = spec * light.specular * vec3(texture(material.diffuseMap, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
