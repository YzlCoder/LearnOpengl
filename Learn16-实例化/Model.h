#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
public:
	/*  ��Ա����   */
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  ģ������  */
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	/*  ˽�г�Ա����   */
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};