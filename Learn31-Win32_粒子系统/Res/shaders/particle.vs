attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;

uniform	mat4 model;
uniform	mat4 view;
uniform mat4 project;

varying vec4  V_Color;

void main()
{
	V_Color = color;
	gl_PointSize = 32.0;
	vec4 pos = vec4(position.xyz + normal.xyz, 1.0f);
	gl_Position = project * view * model * pos;
}