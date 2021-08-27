#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "VAO.h"
#include "Program.h"
#include "GL/Texture.h"

namespace GL
{
	class Model
	{
	private:
		std::vector<VAO*> meshes;
		std::string directory;
		Texture* diffuse;
		Texture* specular;
		Program *shader;
		GLuint MaterialDiffuseID;
		GLuint MaterialSpecularID;
		GLuint MaterialShininessID;
		GLuint TextureSamplerID;
		void processNode(aiNode* node, const aiScene* scene);
		void addMesh(aiMesh* mesh, const aiScene* scene);
		void getUniformsLocation();
	public:
		Model(std::string modelPath, Texture* diffuse, Texture* specular, Program* shader);
		void Draw();
		~Model();
	};
}