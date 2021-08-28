#include "Window.h"
#include "Game/MainMenu.h"
#include "Game/EnterIPScene.h"
#include "Image/stb_image.h"
#include "GL/Model.h"
#include "GLFW/Text.h"

int lastPressedButton = -1;

/**
 * \param title Заголовок окна
 * \param width Ширина окна в пикселях
 * \param height Высота окна в пикселях
 */
Window::Window(const std::string& title, int width, int height)
{
	//glfwGetPrimaryMonitor()
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		throw std::runtime_error("Could not initialize mWindow");
	}
	
	setContextCurrent();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Could not initialize GLAD");
	}

	windowWidth = width;
	windowHeight = height;

	stbi_set_flip_vertically_on_load(false);
}

void Window::setContextCurrent()
{
	glfwMakeContextCurrent(mWindow);
}

void Window::getWindowSize(int* width, int* height)
{
	glfwGetWindowSize(mWindow ,width, height);
}

/**
 * \brief Переводит экранные координаты в мировые, возвращает
 * вектор, состоящий из векторов, по нулевому индексу начальная координата луча,
 * по первому - конечная
 * \param xpos Координата по оси Ox
 * \param ypos Координата по оси Oy
 */
std::vector<glm::vec3> Window::translateToWorldCoord(double xpos, double ypos)
{
	getWindowSize(&windowWidth, &windowHeight);
	
	// перевод координат экрана в мировые координаты
	glm::vec4 vEye(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 0, 1);
	glm::vec4 vDest(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 1, 1);

	glm::mat4 ViewProj = scene->Projection * scene->View;
	ViewProj = glm::inverse(ViewProj);
	vEye = ViewProj * vEye;
	vDest = ViewProj * vDest;

	glm::vec3 rayStart = glm::vec3(vEye.x, vEye.y, vEye.z) / vEye.w;
	glm::vec3 rayEnd = glm::vec3(vDest.x, vDest.y, vDest.z) / vDest.w;

	std::vector<glm::vec3> result;
	result.push_back(rayStart);
	result.push_back(rayEnd);
	return result;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		return;
	}
	
	lastPressedButton = key;
}

/**
 * \brief Обрабатывает нажатите клавиш клавиатуры
 */
void Window::processKeyboardInput()
{
	if (lastPressedButton == -1)
	{
		return;
	}

	scene->ProcessKeyboardInput(lastPressedButton);
	lastPressedButton = -1;
}

/**
 * \brief Обрабатывает нажатите кнопок мыши
 */
void Window::processMouseInput()
{
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		buttonPressed = true;
		return;
	}

	if (!buttonPressed)
	{
		return;
	}
	
	double xpos;
	double ypos;
	glfwGetCursorPos(mWindow, &xpos, &ypos);
	std::vector<glm::vec3> ray = translateToWorldCoord(xpos, ypos);
	scene->ProcessMouseInput(ray);
	buttonPressed = false;
}

GLuint loadTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		throw std::runtime_error("Could not load texture " + path);
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

void Window::switchScene()
{
	if (scene->IsSceneFinished && scene->NextScene != nullptr)
	{
		Scene* lastScene = scene;
		scene = scene->NextScene;
		delete(lastScene);
	}
}

/**
 * \brief Главный цикл программы
 */
void Window::loop()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL::Program shader("3dShader");
	shader.bindAttribute(0, "position");
	shader.bindAttribute(1, "textureCoords");
	shader.bindAttribute(2, "normal");
	shader.link();
	shader.use();
	glm::vec3 lightAmbient = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(0, -1, -0);
	glUniform3fv(shader.GetUinformLacation("LightProp.direction"), 1, &lightDirection[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.specular"), 1, &lightSpecular[0]);
	
	stbi_set_flip_vertically_on_load(true);

	Text text("fonts/VERDANA.TTF", "textShader", glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, -50.0f, 50.0f));
	//std::string ip;
	//std::cin >> ip;
	//if (ip.length() == 1)
	//{
	//	scene = new Game(&shader, &text, windowWidth, windowHeight);
	//}
	//else
	//{
	//	scene = new Game(ip, &shader, &text, windowWidth, windowHeight);
	//}

	glfwSetKeyCallback(mWindow, key_callback);
	scene = new MainMenu(&shader, &text, windowWidth, windowHeight);
	while (!glfwWindowShouldClose(mWindow) && !scene->IsSceneFinished)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		scene->Draw();
		//text.RenderText("hello", 2, 2, 0.05f, glm::vec3(1.0f, 0.0f, 1.0f));
		glfwSwapBuffers(mWindow);
		processKeyboardInput();
		processMouseInput();
		glfwPollEvents();

		if (scene->IsSceneFinished)
		{
			switchScene();
		}
	}

	delete scene;
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
}
