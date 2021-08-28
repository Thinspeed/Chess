#include "Scene.h"

Scene::Scene(GL::Program* shader, Text* textPrinter, int windowWidth, int windowHeight)
{
	Scene::shader = shader;
	Scene::textPrinter = textPrinter;
	Scene::windowWidth = windowWidth;
	Scene::windowHeight = windowHeight;
	shader->use();
	GetUniformsLocation();
	SetUniforms();
}

/**
 * \brief Получает ID юниформ из шейдара
 */
void Scene::GetUniformsLocation()
{
	ViewMatrixID = shader->GetUinformLacation("ViewMatrix");
	ProjectionMatrixID = shader->GetUinformLacation("ProjectionMatrix");
	ViewPosID = shader->GetUinformLacation("ViewPos");
	ModelMatrixID = shader->GetUinformLacation("ModelMatrix");
}

/**
 * \brief Устанавливает стандартные значения для матриц вида и проекции
 */
void Scene::SetUniforms()
{
	//Projection = glm::ortho(left, right, bottom, top, -50.0f, 50.0f);
	Projection = glm::perspective(glm::radians(60.0f), windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
	glm::vec3 viewPos = glm::vec3(2.1f, 4, 1.5f);
	glm::vec3 cameraDir = glm::vec3(2.1f, 0, -2.1);
	View = glm::lookAt(
		viewPos, // координаты камеры
		cameraDir, // направление камеры
		glm::vec3(0, 1, 0)  // вектор, указывающий напрвление вверх
	);

	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniform3fv(ViewPosID, 1, &viewPos[0]);
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
}

void Scene::ProcessMouseInput(std::vector<glm::vec3> ray)
{
}

void Scene::ProcessKeyboardInput(int key)
{
}

void Scene::Draw()
{
}

