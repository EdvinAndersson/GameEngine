#include "shader.h"

namespace CW {

    std::string LoadShaderFromFile(const char *vSource);

    void Shader::Use() {
        glUseProgram(id);
    }

    void Shader::SetV4(const char* name, float x, float y, float z, float w) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform4f(loc, x, y, z, w);
    }
    void Shader::SetV4(const char* name, Vec4 vec4) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
    }
    void Shader::SetV3(const char* name, float x, float y, float z) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform3f(loc, x, y, z);
    }
    void Shader::SetV3(const char *name, Vec3 vec3) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform3f(loc, vec3.x, vec3.y, vec3.z);
    }
    void Shader::SetV2(const char* name, float x, float y) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform2f(loc, x, y);
    }
    void Shader::SetMat4f(const char* name, Mat4* mat) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)mat);
    }
    void Shader::SetFloat(const char* name, float a) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform1f(loc, a);
    }
    void Shader::SetInt(const char* name, int a) {
        unsigned int loc = glGetUniformLocation(id, name);
        glUniform1i(loc, a);
    }
    
    Shader CreateShaderFromFiles(const char *vSource, const char *fSource) {
        return CreateShader(LoadShaderFromFile(vSource), LoadShaderFromFile(fSource));
    }
    Shader CreateShader(std::string vertex_shader, std::string fragment_shader) {

        //----VERTEX SHADER----
        const char *vertex_shader_str = vertex_shader.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShader, 1, &vertex_shader_str, NULL);
        glCompileShader(vertexShader);

        {
            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
                printf("Shader: %s\n", vertex_shader_str);
            }
        }

        //----FRAGMENT SHADER----
        const char *fragment_shader_str = fragment_shader.c_str();

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragment_shader_str, NULL);
        glCompileShader(fragmentShader);

        {
            int success;
            char infoLog[512];
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
                printf("Shader: %s\n", fragment_shader_str);
            }
        }

        //----LINK SHADERS----
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        {
            int success;
            char infoLog[512];
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                printf("ERROR::SHADER::LINKING::COMPILATION_FAILED\n%s\n", infoLog);
            }
        }
        //END
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Shader shader = { shaderProgram };
        return shader;
    }

    std::string LoadShaderFromFile(const char *vSource) {
        std::string total;
        std::fstream vertex_source_file;
        vertex_source_file.open(vSource, std::ios::in);
        if (vertex_source_file.is_open()) {
            std::string vertex_source;
            while (std::getline(vertex_source_file, vertex_source)) {
                total += vertex_source + "\n";
            }
        }
        return total;
    }

}