#pragma once

#include <sstream>
#include <string>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp Includes 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderTool.h"

struct Vertex
{
	glm::vec4 Position;
	glm::vec4 Normal;
	glm::vec4 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;	// We store the path of the texture to compare with other textures
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
	void Draw(Shader shader);

private:
	GLuint VAO, VBO, EBO, SSBO;
	void setupMesh();
};