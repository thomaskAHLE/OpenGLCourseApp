#version 330						
in vec4 vCol;	
in vec2 texCoord;
in vec3 Normal;								
in vec3 FragPos;

out vec4 color;	

const int MAX_POINT_LIGHTS = 3;

struct Light{
	vec3 color;
	float diffuseIntensity;
	float ambientIntensity;
};
struct DirectionalLight {
	Light base;	
	vec3 direction;
};

struct PointLight{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float quadratic;	
};

struct Material{
	float specularIntensity;
	float shininess;	
};


uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;

uniform Material material;
uniform sampler2D theTexture;	

uniform vec3 eyePosition;
	
vec4 CalcLightByDirection(Light light, vec3 direction)
{
 	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0f);	
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
	if( diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = reflect(direction, normalize(Normal));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		} 
 
	}	
	return ambientColor + diffuseColor +  specularColor;
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0.0f,0.0f,0.0f,0.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);
		vec4 color = CalcLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].quadratic * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;
		totalColor += color/attenuation;
	}
	return totalColor;
}
										
void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	
	color = texture(theTexture, texCoord) * finalColor; 					
}