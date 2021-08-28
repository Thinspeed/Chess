#include "Model.h"
#include <iostream>
#include "Image/stb_image.h"

//GL::Model::Model(std::string modelPath, std::string textureFolder, Program* shader)
//{
//	meshes = new std::vector<VAO*>;
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_GenNormals);
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::cout << "Importer initialization failed: ";
//		throw std::runtime_error(importer.GetErrorString());
//	}
//
//	processNode(scene->mRootNode, scene);
//	
//	this->shader = shader;
//	diffuse = loadTexture(textureFolder + "/diffuse.jpg");
//	specular = loadTexture(textureFolder + "/specular.jpg");
//	getUniformsLocation();
//}

GL::Model::Model(std::string modelPath, Texture* diffuse, Texture* specular, Program* shader)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Importer initialization failed: ";
		throw std::runtime_error(importer.GetErrorString());
	}

	processNode(scene->mRootNode, scene);
	Model::shader = shader;
	Model::diffuse = diffuse;
	Model::specular = specular;
	getUniformsLocation();

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

void GL::Model::Draw()
{
	shader->use();
	if (diffuse != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse->Id);
		glUniform1f(MaterialDiffuseID, 0);
	}
	
	if (specular != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular->Id);
		glUniform1f(MaterialDiffuseID, 1);
	}

	float shininess = 32.0f;
	glUniform1f(MaterialShininessID, shininess);
	
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw();
	}
}

void GL::Model::getUniformsLocation()
{
	MaterialDiffuseID = shader->GetUinformLacation("ObjMaterial.diffuse");
	MaterialSpecularID = shader->GetUinformLacation("ObjMaterial.specular");
	MaterialShininessID = shader->GetUinformLacation("ObjMaterial.shininess");
}

GL::Model::~Model()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete(meshes[i]);
	}
}