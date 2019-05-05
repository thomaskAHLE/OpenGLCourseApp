#version 330						
in vec4 vCol;	
in vec2 texCoord;
in vec3 Normal;								
in vec3 FragPos;

out vec4 color;	

struct DirectionalLight {
	vec3 color;
	float diffuseIntensity;
	vec3 direction;
	float ambientIntensity;
};

struct Material{
	float specularIntensity;
	float shininess;	
};

uniform sampler2D theTexture;	
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;
											
void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;	
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)),0.0f);	
	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0.0f,0.0f,0.0f,0.0f);
	if( diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = reflect(directionalLight.direction, normalize(Normal));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		} 
 
	}

	color = texture(theTexture, texCoord) * (ambientColor + diffuseColor + specularColor); 					
}