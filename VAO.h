// Clase VAO: gestiona un Vertex Array Object y el enlace de atributos de VBO
#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// Referencia ID para el Vertex Array Object
	GLuint ID;
	// Constructor que genera un ID para el VAO
	VAO();

	// Enlaza un atributo del VBO (posición, color, etc.) al VAO
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Vincula el VAO
	void Bind();
	// Desvincula el VAO
	void Unbind();
	// Elimina el VAO
	void Delete();
};

#endif