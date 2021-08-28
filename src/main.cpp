#define STB_IMAGE_IMPLEMENTATION
#include "Image/stb_image.h"
#include <iostream>
#include "Net/Server.h"
#include "Net/Client.h"
#include "GLFW/GLFW.h"
#include "GLFW/Window.h"
#include <conio.h>

int ScreenWidth = 1920;
int ScreenHeight = 1080;

int main()
{
	try
	{
		stbi_set_flip_vertically_on_load(true);
		GLFW glfw;
		Window gameWindow("Online Chess", ScreenWidth, ScreenHeight);
		gameWindow.loop();
	}
	catch (std::runtime_error e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		Sleep(3000);
	}

	return 0;
}