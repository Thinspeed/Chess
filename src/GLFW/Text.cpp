#include "Text.h"
#include <iostream>
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Text::Text()
{
	if (FT_Init_FreeType(&ft))
	{
		throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
    loadShader();
}

void Text::loadShader()
{
    shader.bindAttribute(0, "vertex");
}

void Text::LoadFont(std::string font)
{
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
	}

    LoadCharacters();
    FT_Done_Face(face);
}

void Text::LoadCharacters()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // отключаем ограничение выравнивания байтов

    for (unsigned int c = 0; c < 256; c++)
    {
        // Загружаем глифы символов 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Генерируем текстуру
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Задаем для текстуры необходимые опции
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Сохраняем символ для последующего использования
        TextChar character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
    	
        chars.insert(std::pair<char, TextChar>(c, character));
    }
}

Text::~Text()
{
    FT_Done_FreeType(ft);
}
