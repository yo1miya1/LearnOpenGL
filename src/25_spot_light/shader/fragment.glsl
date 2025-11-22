#version 330 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3  position;
    vec3  direction;
    float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;
in vec3 outNormal;
in vec3 outFragPos;
in vec2 outUV;



uniform vec3 viewPos;

void main()
{
	vec3	lightDir	= normalize(light.position - outFragPos);
	float	theta		= dot(lightDir, normalize(-light.direction));
	float	epsilon		= light.cutOff - light.outerCutOff;
	float	intensity	= clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
	// light
	float distance    = length(light.position - outFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// ambient
	vec3	ambient		= light.ambient * vec3(texture(material.diffuse, outUV));
	// diffuse
	vec3	norm		= normalize(outNormal);
	float	diff		= max(dot(norm, lightDir), 0.0);
	vec3	diffuse		= light.diffuse * diff * vec3(texture(material.diffuse, outUV));
	vec2	uv			= vec2(0.0);
	// specular
	vec3	viewDir		= normalize(viewPos - outFragPos);
	vec3	reflectDir	= reflect(-lightDir, norm);
	float	spec		= pow( max( dot( viewDir , reflectDir ) , 0.0 ) , material.shininess);
	vec3	specular	= light.specular * (spec * vec3(texture(material.specular, outUV)));
	// point light
	ambient  *= attenuation; 
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;
	// attenuation
	vec3 result = ambient + diffuse + specular;
	// return
	FragColor = vec4(result, 1.0);
	
}