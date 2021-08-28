#include "EnterIPScene.h"
#include <string>
#include "GLFW/gl.h"
#include "Game/Game.h"

EnterIPScene::EnterIPScene(GL::Program* shader, Text* textPrinter, int windowWidth, int windowHeight) : Scene(shader, textPrinter, windowWidth, windowHeight)
{
}

void EnterIPScene::ProcessKeyboardInput(int key)
{
	if ((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || key == GLFW_KEY_PERIOD)
	{
		if (cursor < ipLength)
		{
			ip[cursor] = key;
			cursor++;
		}
	}
	else if (key == GLFW_KEY_BACKSPACE)
	{
		if (cursor > 0)
		{
			cursor--;
			ip[cursor] = '\0';
		}
	}
	else if (key == GLFW_KEY_ENTER)
	{
		NextScene = new Game(ip, shader, textPrinter, windowWidth, windowHeight);
		IsSceneFinished = true;
	}
}

void EnterIPScene::Draw()
{
	std::string text = std::string(ip);
	clock_t newTime = clock();
	if (newTime - time < 500)
	{
		text += '|';
	}
	else if (newTime - time > 1000)
	{
		time = newTime;
	}

	textPrinter->RenderText(text, 6, 4.5, 0.01f, glm::vec3(1.0f, 0.0f, 1.0f));
	textPrinter->RenderText("connect", 7, 4, 0.01f, glm::vec3(1.0f, 0.44f, 0.22f));
}