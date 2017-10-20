#include <iostream>
#include <fstream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include "LoadShader.h"

using namespace std;

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	// ���û�����ESC, ���ǾͰ�WindowShouldClose����Ϊtrue, �ر�Ӧ��
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static GLuint VAO, VBO, EBO, shaderProgram;

static void RenderInit()
{
	GLfloat vertices[] = {
		// λ��          // ��ɫ
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // ����
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // ��
	};

	GLuint indices[] = {
		0, 1, 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// �������� 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ���� 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	GLuint vertexShader = LoadShader("vertex.shader", GL_VERTEX_SHADER);
	GLuint fragmentShader = LoadShader("fragment.shader", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	LinkShader(shaderProgram, vertexShader, fragmentShader);


}


static void Render()
{
	glUseProgram(shaderProgram);

	// ����uniform��ɫ
	GLfloat timeValue = glfwGetTime();
	GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}


int main()
{
	//��ʼ��glfw����opengl��ص���Ϣ
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//��������
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);


	//��ʼ��glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	RenderInit();

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		// ��鼰�����¼�
		glfwPollEvents();

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();


		//����buffer
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

