#pragma once

#include "Mesh.h"
#include "Texture.h"

#include<vector>
#include<string>

#include<assimp/importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>


class Model
{
public:
	Model();
	void loadModel(const std::string& fileName);
	void renderModel();
	void clearModel();
	~Model();
private:
	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_textures;
	std::vector<unsigned int> m_meshToTex;

	void loadNode(aiNode * node, const aiScene * scene);
	void loadMesh(aiMesh * mesh, const aiScene * scene);
	void loadMaterials(const aiScene * scene);

};

