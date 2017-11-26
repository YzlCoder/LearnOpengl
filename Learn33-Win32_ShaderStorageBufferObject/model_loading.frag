#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

in vec2 TexCoords;

out vec4 color;

uniform Material material;


void main()
{	
    color = vec4(vec3(texture(material.texture_diffuse1, TexCoords)), 1.0f);
}

