#pragma once
#include <iostream>
#include "GLFW/gl.h"

namespace GL
{
	class Texture
	{
	private:
		unsigned int loadTexture(std::string path);
	public:
		GLuint Id;
		Texture(std::string path);
		~Texture();
	};
}