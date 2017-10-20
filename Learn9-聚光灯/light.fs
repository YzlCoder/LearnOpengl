#version 330 core

struct Material 
{ 
    sampler2D diffuse; 
    sampler2D specular; 
    float shininess; 
};

struct Light			// ����һЩ������������Զ�����Ӱ��ÿ������
{
	vec3 position;
    vec3 direction;
    float cutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
		// ��ʽ�ĳ�����һ����Ͷ�����
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
	// ��ambient���ʵ���ɫ����Ϊdiffuse���ʵ���ɫ:
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0, 1.0);

	float diff = max(dot(norm, lightDir), 0.0);
	// �򵥵ش���������������ԭʼ���ص�diffuse��ɫֵ��
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));		// texture����ʹ��ǰ�����õ������������Ӧ��ɫֵ���в���������vec4����

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

