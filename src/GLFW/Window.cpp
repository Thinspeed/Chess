#include "Window.h"
#include "Image/stb_image.h"
#include "GL/Model.h"

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
	std::string ip;
	std::cin >> ip;
	if (ip.length() == 1)
	{
		game = new Game();
	}
	else
	{
		game = new Game(ip);
	}
}

void Window::setContextCurrent()
{
	glfwMakeContextCurrent(mWindow);
}

/**
 * Переводит экранные координаты в мировые
 * \param xpos Координата по оси Ox
 * \param ypos Координата по оси Oy
 */
glm::vec3 Window::translateToWorldCoord(double xpos, double ypos)
{
	// перевод координат экрана в мировые координаты
	glm::vec4 vEye(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 0, 1);
	glm::vec4 vDest(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 0, 1);

	glm::mat4 ViewProj = Projection * View;
	ViewProj = glm::inverse(ViewProj);
	vEye = ViewProj * vEye;
	vDest = ViewProj * vDest;

	// в данном случае координаты x и y совподают у rayStart и rayEnd, поэтому можно использоваь только один из них
	glm::vec3 rayStart = glm::vec3(vEye.x, vEye.y, vEye.z) / vEye.w;
	//glm::vec3 rayEnd = glm::vec3(vDest.x, vDest.y, vDest.z) / vDest.w;

	return rayStart;
}

/**
 * \brief Обрабатывает нажатите клавиш клавиатуры
 */
void Window::processKeyboardInput()
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
	{
		game->FinishGame();
		glfwSetWindowShouldClose(mWindow, true);
	}
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
	glm::vec3 coord = translateToWorldCoord(xpos, ypos);
	if (game->IsMyTurn)
	{
		game->ProcessMapInput(coord.x, coord.y);
	}
	
	buttonPressed = false;
}


/**
 * \brief Получает ID всех юниформ из шейдара
 * \param program Указатель на шейдер
 */
void Window::GetUniformsLocation(GL::Program* program)
{
	ModelMatrixID = program->GetUinformLacation("ModelMatrix");
	ViewMatrixID = program->GetUinformLacation("ViewMatrix");
	ProjectionMatrixID = program->GetUinformLacation("ProjectionMatrix");
	ViewPosID = program->GetUinformLacation("ViewPos");
}

/**
 * \brief Устанавливает стандартные значения для матриц вида и проекции
 */
void Window::SetUniforms()
{
	//Projection = glm::ortho(left, right, bottom, top, -50.0f, 50.0f);
	Projection = glm::perspective(glm::radians(60.0f), windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
	glm::vec3 viewPos = glm::vec3(2.1f, 3, 3);
	glm::vec3 cameraDir = glm::vec3(2.1f, 0, -2.1);
	View = glm::lookAt(
		viewPos, // координаты камеры
		cameraDir, // направление камеры
		glm::vec3(0, 1, 0)  // вектор, указывающий напрвление вверх
	);
	
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniform3fv(ViewPosID, 1, &viewPos[0]);
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


/**
 * \brief Главный цикл программы
 */
void Window::loop()
{
	//GL::Program shader("defaultShader");
	GL::Program shader("3dShader");
	shader.bindAttribute(0, "position");
	shader.bindAttribute(1, "textureCoords");
	shader.bindAttribute(2, "normal");
	shader.link();
	shader.use();
	GetUniformsLocation(&shader);
	SetUniforms();
	glm::vec3 lightAmbient = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(0, -1, -0);
	glUniform3fv(shader.GetUinformLacation("LightProp.direction"), 1, &lightDirection[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(shader.GetUinformLacation("LightProp.specular"), 1, &lightSpecular[0]);
	
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);

	game->chessMap = new Map(0.6f, &shader);
	while (!glfwWindowShouldClose(mWindow) && !game->IsGameFinished)
	{
		if (game->myColor == Color::White)
		{
			glClearColor(1, 1, 1, 1);
		}

		glClearColor(0.5, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		game->chessMap->Draw(ModelMatrixID);
		glfwSwapBuffers(mWindow);
		processKeyboardInput();
		processMouseInput();
		glfwPollEvents();
	}

	delete game;
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
}
