// Clase EBO: encapsula un Element Buffer Object (índices)
#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class EBO
{
public:
	// ID de referencia del Elements Buffer Object
	GLuint ID;
	// Constructor que genera un EBO y lo enlaza a los índices
	EBO(GLuint* indices, GLsizeiptr size);

	// Vincula el EBO
	void Bind();
	// Desvincula el EBO
	void Unbind();
	// Elimina el EBO
	void Delete();
};

#endif