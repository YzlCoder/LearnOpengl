attribute vec4 position;
attribute vec2 texcoord;

uniform	mat4 model;
uniform	mat4 view;
uniform mat4 project;
varying vec2 V_texcoord;
void main()
{
	gl_Position = position;
	V_texcoord = texcoord;
}