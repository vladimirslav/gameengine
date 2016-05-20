#include "graph.h"
#include <fstream>

GLuint LoadVertexShader(const char* file_path)
{
    return 0;
}

bool CompileShader(GLuint shaderId, const char* code)
{

    int InfoLogLength = 0;
    GLint result = GL_FALSE;
    glShaderSource(shaderId, 1, (const GLchar* const*)&code, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        if (VertexShaderErrorMessage[0] != 0)
        {
            EngineRoutines::ShowSimpleMsg(&VertexShaderErrorMessage[0]);
        }
        return false;
    }

    return true;
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);

    if (vertexShaderStream.is_open())
    {
        std::string line;
        while (std::getline(vertexShaderStream, line))
        {
            vertexShaderCode += "\n" + line;
        }
        vertexShaderStream.close();
    }
    else
    {
        // HANDLE ERRORS
        return 0;
    }

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
    if (fragmentShaderStream.is_open())
    {
        std::string line;
        while (std::getline(fragmentShaderStream, line))
        {
            fragmentShaderCode += "\n" + line;
        }
        fragmentShaderStream.close();
    }

    CompileShader(vertexShaderID, vertexShaderCode.c_str());
    CompileShader(fragmentShaderId, fragmentShaderCode.c_str());

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderID);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    GLint result;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    int logLen = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);

    if (logLen > 0)
    {
        std::vector<char> programErrorMessage(logLen + 1);
        glGetShaderInfoLog(programId, logLen, NULL, &programErrorMessage[0]);
        if (programErrorMessage[0] != 0)
        {
            EngineRoutines::ShowSimpleMsg(&programErrorMessage[0]);
        }
    }

    glDetachShader(programId, vertexShaderID);
    glDetachShader(programId, fragmentShaderId);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderId);

    return programId;
}