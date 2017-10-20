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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	float diff = max(dot(norm, lightDir), 0.0);
	// �򵥵ش���������������ԭʼ���ص�diffuse��ɫֵ��
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));		// texture����ʹ��ǰ�����õ������������Ӧ��ɫֵ���в���������vec4����

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}

