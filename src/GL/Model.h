#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "VAO.h"
#include "Program.h"

namespace GL
{
	class Model
	{
	private:
		std::vector<VAO*> meshes;
		std::string directory;
		GLuint color;
		GLuint diffuse;
		GLuint specular;
		Program *shader;
		GLuint MaterialDiffuseID;
		GLuint MaterialSpecularID;
		GLuint MaterialShininessID;
		GLuint TextureSamplerID;
		void processNode(aiNode* node, const aiScene* scene);
		void addMesh(aiMesh* mesh, const aiScene* scene);
		unsigned int loadTexture(std::string path);
	public:
		Model(std::string path, std::string texturePath, Program *shader);
		void Draw();
	};
}