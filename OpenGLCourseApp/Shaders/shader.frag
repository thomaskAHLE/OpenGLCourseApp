#version 330						
in vec4 vCol;	
in vec2 texCoord;
in vec3 Normal;								
in vec3 FragPos;

out vec4 color;	

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material{
	float specularIntensity;
	float shininess;	
};


uniform DirectionalLight directionalLight;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int spotLightCount;

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

vec4 CalcPointLight(PointLight pLight)
{
		vec3 direction = FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);
		vec4 color = CalcLightByDirection(pLight.base, direction);
		float attenuation = pLight.quadratic * distance * distance + pLight.linear * distance + pLight.constant;
		return color/attenuation;
}


vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0.0f,0.0f,0.0f,0.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i]);
	}
	return totalColor;
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);
		float spotLightFade = 1.0f - (1.0f - slFactor)/(1.0f - sLight.edge);
		return color * spotLightFade;
	}
	return vec4(0.0f,0.0f,0.0f,0.0f);
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0.0f,0.0f,0.0f,0.0f);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalcSpotLight(spotLights[i]);
	}
	return totalColor;
}										
void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
	
	color = texture(theTexture, texCoord) * finalColor; 					
}