#pragma once
#include <ctime>
#include "GL/Scene.h"
#include "GL/Model.h"

class MainMenu :
    public Scene
{
private:
	const static int ipLength = 15;
	char ip[ipLength] = { '\0' };
	int cursor = 0;
	clock_t time;
	GL::Texture* newGameDiffuse;
	GL::Texture* connectDiffuse;
	GL::Model *newGameButton;
	GL::Model *connectButton;
	void SetUniforms();
public:
	MainMenu(GL::Program* shader, Text* textPrinter, int windowWidth, int windowHeight);
	void ProcessKeyboardInput(int key) override;
	void ProcessMouseInput(std::vector<glm::vec3> ray) override;
	void Draw() override;
	~MainMenu();
};

