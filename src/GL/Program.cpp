#include "Program.h"
#include <iterator>
#include <fstream>
#include <iostream>

GL::Program::Program(const std::string& name)
{
	mProgram = glCreateProgram();
	mVertexShader = loadShader("res/glsl/" + name + ".vert", GL_VERTEX_SHADER);
	mFragmentShader = loadShader("res/glsl/" + name + ".frag", GL_FRAGMENT_SHADER);
}

/**
 * \brief Последний этап подготовки шейдерной программы. До вызова этой функции необходимо
 * объявить все атрибуты
 */
void GL::Program::link()
{
	// связываем шейдеы с программой
	glAttachShader(mProgram, mVertexShader);
	glAttachShader(mProgram, mFragmentShader);

	// линкуем
	glLinkProgram(mProgram);
}

/**
 * \brief Объявить атрибут. Вызывать строго до
 * \ref link
 */
void GL::Program::bindAttribute(GLuint index, const std::string& name)
{
	glBindAttribLocation(mProgram, index, name.c_str());
}

/**
 * \brief Активировать шейдерную программу
 */
void GL::Program::use()
{
	glUseProgram(mProgram);
}

/**
 * \brief Установить значение юниформа
 * \param name Имя юноформа
 * \parmm value Значение, которое будет присвоено
 */
void GL::Program::setFloat(const std::string& name, float value)
{
	glUniform1f(getLocation(name), value);
}

/**
 * \brief Получить адрес юниформа
 * \param name Имя юноформы
 */
GLuint GL::Program::GetUinformLacation(const std::string& name)
{
	return getLocation(name);
}

/**
 * \brief Получить ключ атрибута для glUniform
 * \param name Название атрибута
 * \return Ключ
 */
GLint GL::Program::getLocation(const std::string& name)
{
	return glGetUniformLocation(mProgram, name.c_str());
}

/**
 * \brief Загрузить шейдер из файла с заданным типом
 * \param path Путь к файлу
 * \param shaderType Тип шейдера
 * \return Хендл
 */
GLuint GL::Program::loadShader(const std::string& path, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	// загрузка кода шейдера
	std::ifstream fis(path);
	std::string shaderCode = { std::istreambuf_iterator<char>(fis), std::istreambuf_iterator<char>() };
	const char* c = shaderCode.c_str();
	glShaderSource(shader, 1, &c, nullptr);

	// скомпилировать шейдер
	glCompileShader(shader);

	// получить результат компиляции (1 = успешно; 0 = неуспешно)
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	char buf[0x1000];
	GLsizei len;
	glGetShaderInfoLog(shader, sizeof(buf), &len, buf);

	// если есть какие-то ошибки или ворнинги
	if (len > 0)
	{
		std::cout << path << ":" << std::endl << buf;
	}

	if (!status) {
		// если не удалось скомпилировать шейдер, выплюнем исключение
		throw std::runtime_error("could not compile shader: " + path);
	}

	return shader;
}

GL::Program::~Program()
{
	glDetachShader(mProgram, mVertexShader);
	glDetachShader(mProgram, mFragmentShader);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);
	glDeleteProgram(mProgram);
}