// Clase VBO: encapsula la creación y manejo de un Vertex Buffer Object
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VBO
{
public:
	// ID de referencia del Vertex Buffer Object
	GLuint ID;
	// Constructor que genera un VBO y lo enlaza a los vértices
	VBO(GLfloat* vertices, GLsizeiptr size);

	// Vincula el VBO
	void Bind();
	// Desvincula el VBO
	void Unbind();
	// Elimina el VBO
	void Delete();
};

#endif