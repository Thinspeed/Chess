#pragma once
#include <map>
#include <string>
#include "GL/Program.h"
#include <ft2build.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include FT_FREETYPE_H

struct TextChar {
	unsigned int TextureID; // ID текстуры глифа
	glm::ivec2 Size; // размер глифа
	glm::ivec2 Bearing; // смещение от линии шрифта до верхнего/левого угла глифа
	unsigned int Advance; // смещение до следующего глифа
};

class Text
{
private:
	FT_Library ft;
	FT_Face face;
	void LoadFont(std::string font);
	void loadShader(std::string shaderPath, glm::mat4 projection);
	void LoadCharacters();
	std::map<char, TextChar> chars;
	GL::Program *shader;
public:
	Text(std::string fontPath, std::string shaderPath, glm::mat4 projection);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
	~Text();
};

