#version 330 core

in vec3 TexCoords;		// 用一个三维方向向量来表示Cubemap纹理的坐标

out vec4 color;

uniform samplerCube skybox;	// Cubemap 纹理采样器

void main()
{
	color = texture(skybox, TexCoords);
	//color = vec4(1,1,0,1);
}