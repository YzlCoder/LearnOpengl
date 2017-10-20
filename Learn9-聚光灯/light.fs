#version 330 core

struct Material 
{ 
    sampler2D diffuse; 
    sampler2D specular; 
    float shininess; 
};

struct Light			// 创建一些光的属性来各自独立的影响每个光照
{
	vec3 position;
    vec3 direction;
    float cutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
		// 公式的常量、一次项和二次项
	float constant;
	float linear;
	float quadratic;
};

out vec4 color;
 
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	// Ambient
	// 把ambient材质的颜色设置为diffuse材质的颜色:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0, 1.0);

	float diff = max(dot(norm, lightDir), 0.0);
	// 简单地从纹理采样，来获得原始像素的diffuse颜色值：
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));		// texture函数使用前面设置的纹理参数对相应颜色值进行采样，返回vec4类型

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation; 
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse * intensity + specular * intensity;
	color = vec4(result, 1.0f);
	
}

