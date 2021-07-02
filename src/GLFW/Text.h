#pragma once
#include <map>
#include <string>
#include <glm/vec2.hpp>
#include "Gl/Program.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct TextChar {
	unsigned int TextureID; // ID �������� �����
	glm::ivec2 Size; // ������ �����
	glm::ivec2 Bearing; // �������� �� ����� ������ �� ��������/������ ���� �����
	unsigned int Advance; // �������� �� ���������� �����
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

