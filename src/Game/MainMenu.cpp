#include "MainMenu.h"
#include <string>
#include <ctime>
#include "GLFW/gl.h"
#include "GL/VAO.h"
#include "GL/Texture.h"
#include "Game/Game.h"
#include "Game/EnterIPScene.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

MainMenu::MainMenu(GL::Program* shader, Text* textPrinter, int windowWidth, int windowHeight) : Scene(shader, textPrinter, windowWidth, windowHeight)
{
	SetUniforms();
	newGameDiffuse = new GL::Texture("textures/Buttons/newGame.jpg");
	connectDiffuse = new GL::Texture("textures/Buttons/connect.jpg");
	newGameButton = new GL::Model("models/other_models/button.obj", newGameDiffuse, nullptr, shader);
	connectButton = new GL::Model("models/other_models/button.obj", connectDiffuse, nullptr, shader);
}

void MainMenu::SetUniforms()
{
	Projection = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, -50.0f, 50.0f);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
	glm::vec3 viewPos = glm::vec3(0, 0, 3);
	glm::vec3 cameraDir = glm::vec3(0, 0, 0);
	View = glm::lookAt(
		viewPos, // координаты камеры
		cameraDir, // направление камеры
		glm::vec3(0, 1, 0)  // вектор, указывающий напрвление вверх
	);

	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniform3fv(ViewPosID, 1, &viewPos[0]);
}

void MainMenu::ProcessKeyboardInput(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		IsSceneFinished = true;
	}
}

void MainMenu::ProcessMouseInput(std::vector<glm::vec3> ray)
{
	std::cout << ray[0].x << " " << ray[0].y << std::endl;
	if (ray[0].x > 0.5 && ray[0].x < 3.5 && ray[0].y > 4.7 && ray[0].y < 5.3)
	{
		NextScene = new Game(shader, textPrinter, windowWidth, windowHeight);
		IsSceneFinished = true;
	}

	if (ray[0].x > 0.5 && ray[0].x < 3.5 && ray[0].y > 3.7 && ray[0].y < 4.3)
	{
		NextScene = new EnterIPScene(shader, textPrinter, windowWidth, windowHeight);
		IsSceneFinished = true;
	}
}

void MainMenu::Draw()
{
	glm::mat4 modelMatrix = glm::mat4(1) * glm::translate(glm::vec3(2, 5, 0));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	newGameButton->Draw();
	modelMatrix = glm::mat4(1) * glm::translate(glm::vec3(2, 4, 0));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	connectButton->Draw();
	
	//textPrinter->RenderText("new game", 0, 5, 0.01f, glm::vec3(1.0f, 0.44f, 0.22f));
	//textPrinter->RenderText("connect", 0, 4.5, 0.01f, glm::vec3(1.0f, 0.44f, 0.22f));
}

MainMenu::~MainMenu()
{
	delete(newGameButton);
	delete(connectButton);
	delete(newGameDiffuse);
	delete(connectDiffuse);
}