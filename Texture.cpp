#include"Texture.h"
#include <iostream>

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Asigna el tipo de la textura al objeto
	type = texType;

	// Guarda ancho, alto y número de canales de color de la imagen
	int widthImg, heightImg, numColCh;
	// Invierte la imagen verticalmente para que aparezca correctamente
	stbi_set_flip_vertically_on_load(true);
	// Lee la imagen desde archivo y la carga en bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	if (bytes)
	{
		std::cout << "TEXTURA OK\n";
	}
	else
	{
		std::cout << "TEXTURA FAIL\n";
	}

	// Genera un objeto de textura OpenGL
	glGenTextures(1, &ID);
	// Asigna la textura a una unidad de textura
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configura algoritmos para min/mag filter
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configura cómo se repite la textura
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Opcional: color de borde si se usa GL_CLAMP_TO_BORDER
	float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Asigna la imagen al objeto de textura OpenGL
	glTexImage2D(
		texType,
		0,
		format,
		widthImg,
		heightImg,
		0,
		format,
		pixelType,
		bytes
	);
	// Genera MipMaps
	glGenerateMipmap(texType);

	// Libera los bytes de la imagen (ya están en OpenGL)
	stbi_image_free(bytes);

	// Desvincula la textura para evitar modificaciones accidentales
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}