#include "Model.h"
#include <iostream>
#include "Image/stb_image.h"

GL::Model::Model(std::string modelPath, std::string textureFolder, Program* shader)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Importer initialization failed: ";
		throw std::runtime_error(importer.GetErrorString());
	}

	diffuse = loadTexture(textureFolder + "/diffuse.jpg");
	specular = loadTexture(textureFolder + "/specular.jpg");
	this->shader = shader;
	processNode(scene->mRootNode, scene);
	MaterialDiffuseID = shader->GetUinformLacation("ObjMaterial.diffuse");
	MaterialSpecularID = shader->GetUinformLacation("ObjMaterial.specular");
	MaterialShininessID = shader->GetUinformLacation("ObjMaterial.shininess");
}

void GL::Model::processNode(aiNode* node, const aiScene* scene)
{
    // Обрабатываем все меши (если они есть) у выбранного узла
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        addMesh(mesh, scene);
    }
    // И проделываем то же самое для всех дочерних узлов
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void GL::Model::addMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvCoordinates;
	std::vector<glm::vec3> normals;
    std::vector<unsigned> indices;
	
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
        vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
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
	vao->addVertexBufferObject(normals);
	vao->addIndices(indices);
	vao->genNormals(vertices, indices);
	meshes.push_back(vao);
}

/**
 * \brief Загрузить текстуру
 * \param path Путь к файлу
 */
unsigned int GL::Model::loadTexture(std::string path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Texture failed to load at path: " + path + "\n");
	}

	return textureID;
}

void GL::Model::Draw()
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glUniform1f(MaterialDiffuseID, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular);
	glUniform1f(MaterialDiffuseID, 1);
	float shininess = 32.0f;
	glUniform1f(MaterialShininessID, shininess);
	
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw();
	}
}