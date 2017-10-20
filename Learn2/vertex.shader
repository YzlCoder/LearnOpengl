#version 330 core

layout(location = 0) in vec3 position; // λ�ñ���������positionΪ 0 
layout(location = 1) in vec3 color; // ��ɫ����������positionΪ 1 

out vec3 ourColor; // ��fragment shader���һ����ɫ 

void main()
{
	gl_Position = vec4(position, 1.0);
	ourColor = color; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}