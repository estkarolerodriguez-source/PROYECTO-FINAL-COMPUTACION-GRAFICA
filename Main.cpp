
#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{
	-0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 1.0f,

	 0.5f, -0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 1.0f,


	 0.5f, -0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 1.0f,


	 -0.5f,  0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 1.0f,

	 // Bottom face
	 -0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   1.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,   0.60f, 0.80f, 1.00f,   0.0f, 0.0f
};

GLuint indices[] =
{
	0, 1, 2,   2, 3, 0,       // Front
	4, 5, 6,   6, 7, 4,       // Back
	8, 9,10,  10,11, 8,       // Left
   12,13,14,  14,15,12,       // Right
   16,17,18,  18,19,16,       // Top
   20,21,22,  22,23,20        // Bottom
};

const GLsizei indexCount = sizeof(indices) / sizeof(indices[0]);


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	Shader shaderProgram(
		"C:/Users/kerg2/Downloads/COMP. GRAFICA/TUTORIAL/vertex_shader.glsl",
		"C:/Users/kerg2/Downloads/COMP. GRAFICA/TUTORIAL/fragment_shader.glsl"
	);

	if (shaderProgram.ID == 0) {
	if (shaderProgram.ID == 0) {
		std::cerr << "Shader program failed to create (ID=0). Check shader files and compile/link output.\n";
	}
	else {
		std::cerr << "Shader program ID=" << shaderProgram.ID << " created\n";
	}



	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	std::cerr << "VAO ID=" << VAO1.ID << " VBO ID=" << VBO1.ID << " EBO ID=" << EBO1.ID << " indexCount=" << indexCount << "\n";

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	Texture brickTex("512 x 512.jpg",
		GL_TEXTURE_2D,
		GL_TEXTURE0,
		GL_RGB,
		GL_UNSIGNED_BYTE);

	brickTex.texUnit(shaderProgram, "tex0", 0);

	float rotation = 0.0f;
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glm::vec3 bez_p0(-2.0f, 1.0f, 4.0f);
	glm::vec3 bez_p1(-1.0f, 2.0f, 2.0f);
	glm::vec3 bez_p2(1.0f, 2.0f, 2.0f);
	glm::vec3 bez_p3(2.0f, 1.0f, 4.0f);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));

	camera.SetBezier(bez_p0, bez_p1, bez_p2, bez_p3, glm::vec3(0.0f));

	camera.followMode = false;
	camera.followDuration = 8.0f;

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.98f, 0.87f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		double deltaTime = crntTime - prevTime;
		prevTime = crntTime;
		// rotation speed in degrees per second (adjust to taste)
		const float rotationSpeed = 20.0f;
		rotation += rotationSpeed * (float)deltaTime;

		// Inicializa la matriz model y aplica rotación
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// Envía la matriz model al shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		if (modelLoc != -1)
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Procesa eventos pendientes antes de leer entrada para que el estado de teclas esté actualizado
		glfwPollEvents();
		// Maneja la entrada de la cámara y sube las matrices (view & projection)
		// Siempre llama a Inputs para que los toggles con F funcionen aunque followMode esté activo
		camera.Inputs(window, (float)deltaTime);
		if (camera.followMode)
		{
			double elapsedTotal = crntTime;
			float t = fmod((float)elapsedTotal / camera.followDuration, 1.0f);
			camera.FollowBezier(bez_p0, bez_p1, bez_p2, bez_p3, t, glm::vec3(0.0f));
		}
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "uMVP", model);

		static double lastDebugPrint = 0.0;
		double nowDebug = crntTime;
		if (nowDebug - lastDebugPrint > 1.0)
		{
			std::cerr << "debug: followT=" << camera.followT << " pos=(" << camera.Position.x << "," << camera.Position.y << "," << camera.Position.z << ") followMode=" << (camera.followMode?1:0) << "\n";
			lastDebugPrint = nowDebug;
		}

		
		glUniform1f(uniID, 0.5f);
		brickTex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}



	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	brickTex.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}