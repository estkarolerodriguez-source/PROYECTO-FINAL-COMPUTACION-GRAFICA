#include"Camera.h"

#include<glm/gtx/compatibility.hpp>
#include<iostream>



Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

// Interpolación Bézier cúbica (De Casteljau / base de Bernstein)
void Camera::FollowBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t, const glm::vec3& target)
{
	// Ajusta t a [0,1]
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	// Bernstein polynomials for cubic Bezier
	float u = 1.0f - t;
	float b0 = u * u * u;
	float b1 = 3.0f * u * u * t;
	float b2 = 3.0f * u * t * t;
	float b3 = t * t * t;

	glm::vec3 pos = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;

	// Actualiza la posición de la cámara
	Position = pos;

	// Orienta la cámara para mirar al punto objetivo
	glm::vec3 dir = glm::normalize(target - Position);
	Orientation = dir;
}

void Camera::SetBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& target)
{
	bez_p0 = p0;
	bez_p1 = p1;
	bez_p2 = p2;
	bez_p3 = p3;
	bez_target = target;
	hasCurve = true;
	// Coloca la cámara en la curva en followT actual
	FollowBezier(bez_p0, bez_p1, bez_p2, bez_p3, followT, bez_target);
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, const glm::mat4& model)
{
	// Inicializa matrices (si no, serían la matriz nula)
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Hace que la cámara mire en la dirección correcta desde la posición
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Añade perspectiva a la escena
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Asegura que el shader esté activo antes de establecer uniforms
	shader.Activate();

	// Exporta las matrices view y projection al Vertex Shader usando nombres comunes
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	if (viewLoc != -1)
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projLoc = glGetUniformLocation(shader.ID, "proj");
	if (projLoc != -1)
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Si se proporcionó un nombre de uniform combinado, subir MVP = projection * view * model
	if (uniform != nullptr)
	{
		glm::mat4 mvp = projection * view * model;
		GLint mvpLoc = glGetUniformLocation(shader.ID, uniform);
		if (mvpLoc != -1)
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	}
}



void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
	// Ayuda: trata GLFW_REPEAT como pulsado para que mantener una tecla funcione
	auto isDown = [&](int key)->bool {
		int s = glfwGetKey(window, key);
		return (s == GLFW_PRESS || s == GLFW_REPEAT);
	};

	// Alterna el modo follow con F (solo alterna una vez por pulsación)
	static bool fKeyPressed = false;
	if (isDown(GLFW_KEY_F) && !fKeyPressed)
	{
		followMode = !followMode;
		fKeyPressed = true;
	}
	if (!isDown(GLFW_KEY_F))
	{
		fKeyPressed = false;
	}

	// deltaTime lo proporciona el llamante (bucle principal)
	// Calcula la velocidad; mantener shift aumenta la velocidad
	float speedMultiplier = isDown(GLFW_KEY_LEFT_SHIFT) ? 4.0f : 1.0f;
	float velocity = speed * speedMultiplier * deltaTime;

	// Si hay una curva Bézier almacenada, permitir moverla con las flechas izquierda/derecha
	if (hasCurve)
	{

		bool changed = false;
		if (isDown(GLFW_KEY_RIGHT))
		{
			followT += followSpeed * deltaTime; // advance
			followMode = false; // switch to manual when user controls
			changed = true;
		}
		if (isDown(GLFW_KEY_LEFT))
		{
			followT -= followSpeed * deltaTime; // go back
			followMode = false;
			changed = true;
		}
		// Teclas alternativas si las flechas no se capturan
		if (isDown(GLFW_KEY_E))
		{
			followT += followSpeed * deltaTime; // advance
			followMode = false;
			changed = true;
		}
		if (isDown(GLFW_KEY_Q))
		{
			followT -= followSpeed * deltaTime; // go back
			followMode = false;
			changed = true;
		}
		// Clamp
		if (followT < 0.0f) followT = 0.0f;
		if (followT > 1.0f) followT = 1.0f;
		// Actualiza la posición en la curva solo si el usuario cambió followT aquí.
		if (changed)
		{
			// Clamp
			if (followT < 0.0f) followT = 0.0f;
			if (followT > 1.0f) followT = 1.0f;
			FollowBezier(bez_p0, bez_p1, bez_p2, bez_p3, followT, bez_target);
		}
	}
	else
	{

	}

	// Maneja entradas del teclado (independiente del frame-rate)
	if (isDown(GLFW_KEY_W))
	{
		Position += velocity * Orientation;
	}
	if (isDown(GLFW_KEY_A))
	{
		Position += velocity * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (isDown(GLFW_KEY_S))
	{
		Position += velocity * -Orientation;
	}
	if (isDown(GLFW_KEY_D))
	{
		Position += velocity * glm::normalize(glm::cross(Orientation, Up));
	}
	if (isDown(GLFW_KEY_SPACE))
	{
		Position += velocity * Up;
	}
	if (isDown(GLFW_KEY_LEFT_CONTROL))
	{
		Position += velocity * -Up;
	}

	// Esto permite ver todas las caras sin el ratón
	float rotSpeed = sensitivity * 30.0f; // degrees per second
	if (isDown(GLFW_KEY_J)) {
		float ang = glm::radians(rotSpeed * deltaTime);
		Orientation = glm::rotate(Orientation, ang, Up);
		followMode = false;
	}
	if (isDown(GLFW_KEY_L)) {
		float ang = glm::radians(-rotSpeed * deltaTime);
		Orientation = glm::rotate(Orientation, ang, Up);
		followMode = false;
	}
	if (isDown(GLFW_KEY_I)) {
		float ang = glm::radians(rotSpeed * deltaTime);
		glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
		glm::vec3 newOri = glm::rotate(Orientation, ang, right);
		// limit pitch
		if (abs(glm::angle(newOri, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			Orientation = newOri;
			followMode = false;
		}
	}
	if (isDown(GLFW_KEY_K)) {
		float ang = glm::radians(-rotSpeed * deltaTime);
		glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
		glm::vec3 newOri = glm::rotate(Orientation, ang, right);
		if (abs(glm::angle(newOri, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			Orientation = newOri;
			followMode = false;
		}
	}



	// Maneja la entrada del ratón
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Oculta el cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Evita que la cámara salte en el primer clic
		if (firstClick)
		{
			// coloca el cursor en el centro
			glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
			firstClick = false;
		}

		// Coordenadas del cursor
		double mouseX;
		double mouseY;
		// Obtiene las coordenadas del cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normaliza y traslada de modo que el origen sea el centro de la pantalla
		// y luego lo transforma a grados
		float centerX = (float)width / 2.0f;
		float centerY = (float)height / 2.0f;
		float rotX = sensitivity * (float)(mouseY - centerY) / height;
		float rotY = sensitivity * (float)(mouseX - centerX) / width;

		// Calcula el cambio vertical próximo en la orientación
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decide si la nueva orientación vertical es legal
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rota la orientación a la izquierda y derecha
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Recoloca el cursor en el centro para que no se vaya
		glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Muestra el cursor ya que la cámara no está mirando alrededor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Asegura que la próxima vez que se mire no salte
		firstClick = true;
	}
}