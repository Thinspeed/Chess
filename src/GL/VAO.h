#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace GL
{
	class VAO
	{
	private:
		GLuint mVao;
		std::vector<GLuint> mBuffers;
		unsigned mIndicesBuffer = 0;
		size_t mIndicesCount = 0;
	public:
		VAO();
		VAO(const VAO&) = delete;
		void bind();
		void draw(unsigned type = 0x0004);
		void addVertexBufferObject(const std::vector<float>& data);
		void addVertexBufferObject(const std::vector<glm::vec2>& data);
		void addVertexBufferObject(const std::vector<glm::vec3>& data);
		void addVertexBufferObject(const std::vector<glm::vec4>& data);
		void addIndices(const std::vector<unsigned>& data);
		~VAO();
	};
}

