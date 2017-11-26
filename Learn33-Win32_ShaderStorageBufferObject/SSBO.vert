#version 430 core

struct Vertex
{
	vec4 pos;
	vec4 normal;
	vec4 texCoords;
};

layout(std140,binding=0)buffer Mesh{
	Vertex vertexes[];
}mesh;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(mesh.vertexes[gl_VertexID].pos.xyz, 1.0f);
    TexCoords = mesh.vertexes[gl_VertexID].texCoords.xy;
}