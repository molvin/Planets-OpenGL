#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>

const char* VERTEX_SRC = R"(
#version 330 core

in vec2 a_Position;
in vec3 a_Color;
out vec3 f_Color;

void main()
{
	gl_Position = vec4(a_Position, 0.0f, 1.0f);
	f_Color = a_Color;
}
)";


const char* FRAG_SRC = R"(
#version 330 core

out vec4 o_Color;
in vec3 f_Color;

void main()
{
	o_Color = vec4(f_Color, 1.0f);
}

)";

void OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800	, 600, "GLFW Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//input
	glfwSetKeyCallback(window, OnKeyEvent);

	glewInit();

	//vertices
	float vertices[] =
	{
		-0.5f, -0.5f, 
		0.5f, -0.5f,
		0.0f, 0.5f
	};
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Draw triangle
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Color
	float colors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//Draw triangle
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	//Shader
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VERTEX_SRC, nullptr);
	glCompileShader(vertexShader);
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FRAG_SRC, nullptr);
	glCompileShader(fragmentShader);
	const GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	char infoLogBuffer[1024];
	glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLogBuffer);
	printf("Shader result: \n %s", infoLogBuffer);

	
	while (!glfwWindowShouldClose(window))
	{		
		//Clear
		glfwSwapBuffers(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		glDrawArrays(GL_TRIANGLES, 0, 3);
	
		//Poll events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}