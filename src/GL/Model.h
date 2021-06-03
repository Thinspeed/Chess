#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "VAO.h"

namespace GL
{
	class Model
	{
	private:
		std::vector<VAO*> meshes;
		std::string directory;
		void processNode(aiNode* node, const aiScene* scene);
		void addMesh(aiMesh* mesh, const aiScene* scene);
	public:
		Model(std::string path);
		void Draw();
	};
}