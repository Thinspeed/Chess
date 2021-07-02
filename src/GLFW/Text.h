#pragma once
#include <map>
#include <string>
#include <glm/vec2.hpp>
#include "Gl/Program.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct TextChar {
	unsigned int TextureID; // ID текстуры глифа
	glm::ivec2 Size; // размер глифа
	glm::ivec2 Bearing; // смещение от линии шрифта до верхнего/левого угла глифа
	unsigned int Advance; // смещение до следующего глифа
};

std::map<char, TextChar> chars;

class Text
{
private:
	void loadShader();
public:
	FT_Library ft;
	FT_Face face;
	GL::Program shader = GL::Program("textShader");
	Text();
	void LoadFont(std::string font);
	void LoadCharacters();
	~Text();
};

