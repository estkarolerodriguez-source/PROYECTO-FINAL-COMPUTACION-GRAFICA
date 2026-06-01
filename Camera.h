#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 2.5f;
	float sensitivity = 10.0f;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, const glm::mat4& model = glm::mat4(1.0f));
	void Inputs(GLFWwindow* window, float deltaTime);

	void FollowBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t, const glm::vec3& target);

	void SetBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& target);

	float followT = 0.0f;
	float followSpeed = 1.0f;

	glm::vec3 bez_p0;
	glm::vec3 bez_p1;
	glm::vec3 bez_p2;
	glm::vec3 bez_p3;
	glm::vec3 bez_target;
	bool hasCurve = false;

	bool followMode = false;
	float followDuration = 5.0f;
};

#endif
