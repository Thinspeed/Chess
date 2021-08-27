#include "Text.h"
#include <iostream>
#include <glad/glad.h>
#include "GL/VAO.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Text::Text(std::string fontPath, std::string shaderPath, glm::mat4 projection)
{
	if (FT_Init_FreeType(&ft))
	{
		throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
	}

	LoadFont(fontPath);
	loadShader(shaderPath, projection);
}

void Text::loadShader(std::string shaderPath, glm::mat4 projection)
{
	shader = new GL::Program(shaderPath);
	shader->bindAttribute(0, "vertex");
	shader->link();
	shader->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(shader->GetUinformLacation("projection"), 1, GL_FALSE, &projection[0][0]);
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
	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // отключаем ограничение выравнивания байтов

	for (unsigned int c = 0; c < 256; c++)
	{
		// Загружаем глифы символов
		FT_Error result;
		if (result = FT_Load_Char(face, (char)c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load character " << (char)c << ". Error code: " << result << std::endl;
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

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	// Активируем соответствующее состояние рендеринга	
	shader->use();
	glUniform3fv(shader->GetUinformLacation("textColor"), 1, &color[0]);
	glActiveTexture(GL_TEXTURE0);

	// Перебираем все символы
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		GL::VAO vao;
		TextChar ch = chars[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		vao.addVertexBufferObject({
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos, ypos, 0.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },

			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		});

		vao.addIndices({ 0, 1, 2, 3, 4, 5 });
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1f(shader->GetUinformLacation("text"), 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		vao.draw();

		// Теперь производим смещение для отображения следующего глифа (обратите внимание, что данное смещение измеряется в единицах, составляющих 1/64 пикселя)
		x += (ch.Advance >> 6) * scale; // побитовый сдвиг на 6, чтобы получить значение в пикселях (2^6 = 64)
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Text::~Text()
{
	FT_Done_Face(face);
	delete(shader);
}
