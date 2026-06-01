// Clase Texture: carga y gestiona texturas para OpenGL
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	// Asigna una unidad de textura a un uniform en el shader
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Vincula la textura
	void Bind();
	// Desvincula la textura
	void Unbind();
	// Elimina la textura
	void Delete();
};
#endif