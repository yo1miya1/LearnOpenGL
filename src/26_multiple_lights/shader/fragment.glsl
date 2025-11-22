#version 330 core
// 材质
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
// 定向光
struct DirLight{
	vec3  direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 点光源
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
// 聚光
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


uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[4];
uniform SpotLight spotLight;

// 输入输出
in vec3 outNormal;
in vec3 outFragPos;
in vec2 outUV;
out vec4 FragColor;
// uniform
uniform vec3 viewPos;



// 函数声明
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



void main()
{
	// 属性
    vec3 norm = normalize(outNormal);
    vec3 viewDir = normalize(viewPos - outFragPos);
    vec3 result;
    // 第一阶段：定向光照
    result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < 4; i++)
        result += CalcPointLight(pointLights[i], norm, outFragPos, viewDir); 
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, outFragPos, viewDir);  

    FragColor = vec4(result, 1.0);
}



// 函数定义
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, outUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, outUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, outUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3	lightDir	= normalize(light.position - outFragPos);
	float	theta		= dot(lightDir, normalize(-light.direction));
	float	epsilon		= light.cutOff - light.outerCutOff;
	float	intensity	= clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 衰减
	float distance    = length(light.position - outFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 漫反射着色
	float	diff		= max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3	reflectDir	= reflect(-lightDir, normal);
	float	spec		= pow( max( dot( viewDir , reflectDir ) , 0.0 ) , material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, outUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, outUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, outUV));
	ambient  *= attenuation; 
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;
	// attenuation
    return (ambient + diffuse + specular);
}