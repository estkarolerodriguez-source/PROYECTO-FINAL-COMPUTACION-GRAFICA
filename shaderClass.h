// Clase Shader: encapsula la compilación, enlace y uso de programas de shader
#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
    GLuint ID;
    // Constructor que crea el programa de shader a partir de 2 shaders
    Shader(const char* vertexFile, const char* fragmentFile);

    // Activa el programa de shader
    void Activate();

    // Elimina el programa de shader
    void Delete();

private:
    // Comprueba si los shaders se compilaron correctamente
    void compileErrors(unsigned int shader, const char* type);
};
