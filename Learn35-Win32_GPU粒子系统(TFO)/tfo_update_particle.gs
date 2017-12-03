#version 330 core
layout(points)in;
layout(points, max_vertices = 1)out;

in vec4 vs_Mess[];

out vec4 o_mess;

void main()
{
	float currentAge=vs_Mess[0].x;
	if(currentAge<5.0)
	{
		o_mess=vs_Mess[0];
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
	}
}