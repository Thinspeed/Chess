#include <iostream>
#include "Model.h"

GL::Model::Model(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Importer initialization failed: ";
		throw std::runtime_error(importer.GetErrorString());
	}

	processNode(scene->mRootNode, scene);
}

void GL::Model::processNode(aiNode* node, const aiScene* scene)
{
    // ќбрабатываем все меши (если они есть) у выбранного узла
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        addMesh(mesh, scene);
    }
    // » проделываем то же самое дл€ всех дочерних узлов
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void GL::Model::addMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvCoordinates;
    std::vector<unsigned> indices;
	
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
        vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		if (mesh->mTextureCoords[0])
		{
			uvCoordinates.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}
		else
		{
			uvCoordinates.push_back(glm::vec2(0.0f));
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	VAO* vao = new VAO;
	vao->addVertexBufferObject(vertices);
	vao->addVertexBufferObject(uvCoordinates);
	vao->addIndices(indices);
	meshes.push_back(vao);
}

void GL::Model::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw();
	}
}