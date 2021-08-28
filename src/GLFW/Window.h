#pragma once
#include "gl.h"
#include "GL/Program.h"
#include "Game/Map.h"
#include "GL/Scene.h"
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
	Scene *scene;
	void processKeyboardInput();
	void processMouseInput();
	void getWindowSize(int* width, int* height);
	std::vector<glm::vec3> translateToWorldCoord(double xpos, double ypos);
	void switchScene();
public:
	Window(const std::string& title, int width, int height);
	void setContextCurrent();
	void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void loop();
	~Window();
};