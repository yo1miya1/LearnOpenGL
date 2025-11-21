#version 330 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};
struct Light {
    vec3 position;

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
	// light
	float distance    = length(light.position - outFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// ambient
	vec3	ambient		= light.ambient * vec3(texture(material.diffuse, outUV));
	// diffuse
	vec3	norm		= normalize(outNormal);

	vec3	lightDir;
			lightDir	= normalize(light.position - outFragPos);
	
	float	diff		= max(dot(norm, lightDir), 0.0);
	vec3	diffuse		= light.diffuse * diff * vec3(texture(material.diffuse, outUV));
	vec2	uv			= vec2(0.0);
	vec3	emissive	= vec3(texture(material.emissive, uv));
	// specular
	vec3	viewDir		= normalize(viewPos - outFragPos);
	vec3	reflectDir	= reflect(-lightDir, norm);
	float	spec		= pow( max( dot( viewDir , reflectDir ) , 0.0 ) , material.shininess);
	vec3	specular	= light.specular * (spec * vec3(texture(material.specular, outUV)));
	// point light
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	// attenuation
	vec3 result = ambient + diffuse + specular + emissive;
	// return
	FragColor = vec4(result, 1.0);
}