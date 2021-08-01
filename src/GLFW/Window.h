#pragma once
#include "gl.h"
#include "GL/Program.h"
#include "Game/Map.h"
#include "Game/Game.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define left 0.0f
#define right 16.0f
#define bottom 0.0f
#define top 9.0f

class Window
{
private:
	int windowWidth;
	int windowHeight;
	bool buttonPressed = false;
	GLFWwindow* mWindow;
	glm::mat4 View;
	glm::mat4 Projection;
	Game *game;
	void processKeyboardInput();
	void processMouseInput();
	std::vector<glm::vec3> translateToWorldCoord(double xpos, double ypos);
	void GetUniformsLocation(GL::Program *program);
	void SetUniforms();
public:
	GLuint ModelMatrixID;
	GLuint ViewMatrixID;
	GLuint ProjectionMatrixID;
	GLuint ViewPosID;
	Window(const std::string& title, int width, int height);
	void setContextCurrent();
	void loop();
	~Window();
};