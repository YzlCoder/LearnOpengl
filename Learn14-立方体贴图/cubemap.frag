#version 330 core

in vec3 TexCoords;		// ��һ����ά������������ʾCubemap���������

out vec4 color;

uniform samplerCube skybox;	// Cubemap ���������

void main()
{
	color = texture(skybox, TexCoords);
	//color = vec4(1,1,0,1);
}