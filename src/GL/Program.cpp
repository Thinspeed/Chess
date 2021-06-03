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
 * \brief ��������� ���� ���������� ��������� ���������. �� ������ ���� ������� ����������
 * �������� ��� ��������
 */
void GL::Program::link()
{
	// ��������� ������ � ����������
	glAttachShader(mProgram, mVertexShader);
	glAttachShader(mProgram, mFragmentShader);

	// �������
	glLinkProgram(mProgram);
}

/**
 * \brief �������� �������. �������� ������ ��
 * \ref link
 */
void GL::Program::bindAttribute(GLuint index, const std::string& name)
{
	glBindAttribLocation(mProgram, index, name.c_str());
}

/**
 * \brief ������������ ��������� ���������
 */
void GL::Program::use()
{
	glUseProgram(mProgram);
}

/**
 * \brief ���������� �������� ��������
 * \param name ��� ��������
 * \parmm value ��������, ������� ����� ���������
 */
void GL::Program::setFloat(const std::string& name, float value)
{
	glUniform1f(getLocation(name), value);
}

/**
 * \brief �������� ����� ��������
 * \param name ��� ��������
 */
GLuint GL::Program::GetUinformLacation(const std::string& name)
{
	return getLocation(name);
}

/**
 * \brief �������� ���� �������� ��� glUniform
 * \param name �������� ��������
 * \return ����
 */
GLint GL::Program::getLocation(const std::string& name)
{
	return glGetUniformLocation(mProgram, name.c_str());
}

/**
 * \brief ��������� ������ �� ����� � �������� �����
 * \param path ���� � �����
 * \param shaderType ��� �������
 * \return �����
 */
GLuint GL::Program::loadShader(const std::string& path, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	// �������� ���� �������
	std::ifstream fis(path);
	std::string shaderCode = { std::istreambuf_iterator<char>(fis), std::istreambuf_iterator<char>() };
	const char* c = shaderCode.c_str();
	glShaderSource(shader, 1, &c, nullptr);

	// �������������� ������
	glCompileShader(shader);

	// �������� ��������� ���������� (1 = �������; 0 = ���������)
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	char buf[0x1000];
	GLsizei len;
	glGetShaderInfoLog(shader, sizeof(buf), &len, buf);

	// ���� ���� �����-�� ������ ��� ��������
	if (len > 0)
	{
		std::cout << path << ":" << std::endl << buf;
	}

	if (!status) {
		// ���� �� ������� �������������� ������, �������� ����������
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