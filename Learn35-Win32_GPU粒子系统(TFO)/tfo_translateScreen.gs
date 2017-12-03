#version 330 core
layout(points)in;
layout(triangle_strip, max_vertices = 4)out;

uniform mat4 P;

in vec4 vs_Color[];
in vec4 vs_Mess[];

out vec4 V_Color;
out vec2 V_Texcoord;

void main()
{
	vec4 pos=gl_in[0].gl_Position;//view coordinate
	float spriteSize = 0.1;

	gl_Position = P*(pos + vec4(-spriteSize,-spriteSize,0.0,1.0));
	V_Texcoord = vec2(0.0,0.0);
	V_Color=vs_Color[0];
	EmitVertex();

	gl_Position = P*(pos + vec4(spriteSize, -spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 0.0);
	V_Color=vs_Color[0];
	EmitVertex();

	gl_Position = P*(pos + vec4(-spriteSize, spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(0.0, 1.0);
	V_Color=vs_Color[0];
	EmitVertex();

	gl_Position = P*(pos + vec4(spriteSize, spriteSize, 0.0, 1.0));
	V_Texcoord = vec2(1.0, 1.0);
	V_Color=vs_Color[0];
	EmitVertex();

	EndPrimitive();
}