#include "Model.h"



Model::Model()
{
}

void Model::loadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		printf("Model %s failed to load %s \n", fileName,  importer.GetErrorString());
		return;
	}
	loadNode(scene->mRootNode, scene);
	loadMaterials(scene);

}

void Model::renderModel()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		unsigned int materialIndex = m_meshToTex[i];
		if (materialIndex < m_textures.size() && m_textures[materialIndex])
		{
			m_textures[materialIndex]->useTexture();
		}
		m_meshes[i]->renderMesh();
	}
}

void Model::clearModel()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		if (m_meshes[i])
		{
			delete m_meshes[i];
			m_meshes[i] = nullptr;
		}
	}

	for (size_t i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i])
		{
			delete m_textures[i];
			m_textures[i] = nullptr;
		}
	}
}


Model::~Model()
{
	clearModel();
}

void Model::loadNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		loadNode(node->mChildren[i], scene);
	}
}

void Model::loadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else 
		{
			vertices.insert(vertices.end(), {0.0f ,0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	Mesh* m = new Mesh();
	m->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	m_meshes.push_back(m);
	m_meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene * scene)
{
	m_textures.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		m_textures[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind('\\');
				std::string fileName = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("textures/") + fileName;
				m_textures[i] = new Texture(texPath.c_str());
				if (!m_textures[i]->loadTexture())
				{
					printf("Falied to load texture at %s \n", texPath);
					delete m_textures[i];
					m_textures[i] = nullptr;
				}
			}
		}
		if (!m_textures[i])
		{
			m_textures[i] = new Texture("Textures/plain.png");
			m_textures[i]->loadTextureAlpha();
		}
	}
}
