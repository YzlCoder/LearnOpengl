#version 330 core
in vec3 ourColor;//这是你要的分号
out vec4 color;
void main()
{
	color = vec4(ourColor, 1.0f);
}