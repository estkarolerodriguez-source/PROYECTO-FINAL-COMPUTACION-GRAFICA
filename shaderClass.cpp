#include "shaderClass.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cerrno>
#include <filesystem>

// Versión segura de strerror para MSVC
static std::string safe_strerror(int err)
{
    char buf[256];
#ifdef _MSC_VER
    strerror_s(buf, sizeof(buf), err);
    return std::string(buf);
#elif defined(__GLIBC__)
    if (strerror_r(err, buf, sizeof(buf)) == 0) return std::string(buf);
    return std::string("Unknown error");
#else
    const char* s = std::strerror(err);
    return s ? std::string(s) : std::string("Unknown error");
#endif
}

// Lee un archivo de texto y devuelve su contenido como string
static std::string get_file_contents(const char* filename)
{
    try {
        if (!std::filesystem::exists(filename)) {
            std::cerr << "get_file_contents: el archivo no existe: " << filename << '\n';
            return std::string();
        }
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open()) {
            int err = errno;
            std::cerr << "get_file_contents: fallo al abrir el archivo '" << filename << "' (errno=" << err << "): " << safe_strerror(err) << '\n';
            return std::string();
        }
        std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        return contents;
    } catch (const std::exception &e) {
        std::cerr << "get_file_contents excepción: " << e.what() << '\n';
        return std::string();
    }
}

// Constructor que crea el programa de shader a partir de 2 archivos shader
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{

    const char *fallbackVert = "#version 330 core\nlayout(location=0) in vec3 aPos;\nlayout(location=1) in vec3 aColor;\nlayout(location=2) in vec2 aTexCoord;\nout vec3 fColor;\nout vec2 fTexCoord;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 proj;\nvoid main(){ fColor = aColor; fTexCoord = aTexCoord; gl_Position = proj * view * model * vec4(aPos,1.0); }\n";
    const char *fallbackFrag = "#version 330 core\nin vec3 fColor;\nin vec2 fTexCoord;\nout vec4 FragColor;\nvoid main(){ FragColor = vec4(fColor, 1.0); }\n";

    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.empty() ? fallbackVert : vertexCode.c_str();
    const char* fragmentSource = fragmentCode.empty() ? fallbackFrag : fragmentCode.c_str();

    auto compile = [&](GLenum type, const char* src)->GLuint {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, NULL);
        glCompileShader(s);
        GLint ok = 0;
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char info[1024];
            glGetShaderInfoLog(s, 1024, NULL, info);
            std::cerr << "Shader compile error (type=" << type << "): " << info << '\n';
            glDeleteShader(s);
            return 0;
        }
        return s;
    };

    GLuint vShader = compile(GL_VERTEX_SHADER, vertexSource);
    if (!vShader && vertexSource != fallbackVert) {
        std::cerr << "Falling back to embedded vertex shader\n";
        vShader = compile(GL_VERTEX_SHADER, fallbackVert);
    }
    GLuint fShader = compile(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fShader && fragmentSource != fallbackFrag) {
        std::cerr << "Falling back to embedded fragment shader\n";
        fShader = compile(GL_FRAGMENT_SHADER, fallbackFrag);
    }

    if (!vShader || !fShader) {
        std::cerr << "Fallo al compilar shaders, el programa será inválido\n";
        ID = 0;
        if (vShader) glDeleteShader(vShader);
        if (fShader) glDeleteShader(fShader);
        return;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);
    GLint ok = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &ok);
    if (!ok) {
        char info[1024];
        glGetProgramInfoLog(ID, 1024, NULL, info);
        std::cerr << "Error al enlazar el programa: " << info << '\n';
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        glDeleteProgram(ID);
        ID = 0;
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

// Activa el programa de shader
void Shader::Activate()
{
	glUseProgram(ID);
}

// Elimina el programa de shader
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Comprueba si los shaders se compilaron correctamente
void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR_COMPILACION_SHADER para:" << type << "\n" << infoLog << std::endl;
        }
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR_ENLACE_SHADER para:" << type << "\n" << infoLog << std::endl;
		}
	}
}