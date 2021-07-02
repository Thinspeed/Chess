#include "Window.h"
#include "Image/stb_image.h"

/**
 * \param title ��������� ����
 * \param width ������ ���� � ��������
 * \param height ������ ���� � ��������
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

	game->chessMap = new Map((top - bottom) / 8);
}

void Window::setContextCurrent()
{
	glfwMakeContextCurrent(mWindow);
}

/**
 * ��������� �������� ���������� � �������
 * \param xpos ���������� �� ��� Ox
 * \param ypos ���������� �� ��� Oy
 */
glm::vec3 Window::translateToWorldCoord(double xpos, double ypos)
{
	// ������� ��������� ������ � ������� ����������
	glm::vec4 vEye(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 0, 1);
	glm::vec4 vDest(xpos / windowWidth * 2 - 1, 1 - ypos / windowHeight * 2, 0, 1);

	glm::mat4 ViewProj = Projection * View;
	ViewProj = glm::inverse(ViewProj);
	vEye = ViewProj * vEye;
	vDest = ViewProj * vDest;

	// � ������ ������ ���������� x � y ��������� � rayStart � rayEnd, ������� ����� ����������� ������ ���� �� ���
	glm::vec3 rayStart = glm::vec3(vEye.x, vEye.y, vEye.z) / vEye.w;
	//glm::vec3 rayEnd = glm::vec3(vDest.x, vDest.y, vDest.z) / vDest.w;

	return rayStart;
}

/**
 * \brief ������������ �������� ������ ����������
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
 * \brief ������������ �������� ������ ����
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
 * \brief �������� ID ���� ������� �� �������
 * \param program ��������� �� ������
 */
void Window::GetAllUniformLocation(GL::Program* program)
{
	ModelMatrixID = program->GetUinformLacation("ModelMatrix");
	ViewMatrixID = program->GetUinformLacation("ViewMatrix");
	ProjectionMatrixID = program->GetUinformLacation("ProjectionMatrix");
	LightPosID = program->GetUinformLacation("LightPos");
	LightColorID = program->GetUinformLacation("LightColor");
}

/**
 * \brief ������������� ����������� �������� ��� ������ ���� � ��������
 */
void Window::SetViewProjectionMatrix()
{
	Projection = glm::ortho(left, right, bottom, top, -50.0f, 50.0f);
	//Projection = glm::perspective(glm::radians(45.0f), windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
	View = glm::lookAt(
		glm::vec3(0, 0, 3), // ���������� ������
		glm::vec3(0, 0, 0), // ����������� ������
		glm::vec3(0, 1, 0)  // ������, ����������� ���������� �����
	);

	
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix3fv(LightPosID, 1, GL_FALSE, &View[0][0]);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(LightColorID, 1, &lightColor[0]);
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
 * \brief ������� ���� ���������
 */
void Window::loop()
{
	GL::Program shader("defaultShader");
	shader.bindAttribute(0, "position");
	shader.bindAttribute(1, "textureCoords");
	shader.link();
	shader.use();

	GetAllUniformLocation(&shader);
	SetViewProjectionMatrix();
	
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);

	while (!glfwWindowShouldClose(mWindow) && !game->IsGameFinished)
	{
		if (game->myColor == Color::White)
		{
			glClearColor(1, 1, 1, 1);
		}
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
