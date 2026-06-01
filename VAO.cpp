#include"VAO.h"

// Constructor que genera un ID para el VAO
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Enlaza un atributo del VBO (posición, color, etc.) al VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}

// Vincula el VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Desvincula el VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Elimina el VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
