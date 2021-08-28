#pragma once
#include <vector>
#include "Program.h"
#include "GLFW/Text.h"

class Scene
{
protected:
	GL::Program* shader;
	Text* textPrinter;
	GLuint ViewMatrixID;
	GLuint ViewPosID;
	GLuint ProjectionMatrixID;
	GLuint ModelMatrixID;
	int windowWidth;
	int windowHeight;
	void GetUniformsLocation();
	void SetUniforms();
public:
	Scene* NextScene;
	glm::mat4 View;
	glm::mat4 Projection;
	bool IsSceneFinished = false;
	Scene(GL::Program* shader, Text* textPrinter, int windowWidth, int windowHeight);
	void virtual ProcessMouseInput(std::vector<glm::vec3> ray);
	void virtual ProcessKeyboardInput(int key);
	void virtual Draw();
};

