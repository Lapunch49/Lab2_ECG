#include <iostream>
#include <GL\glew.h>
//#include <GL/glut.h>
#include <GL\freeglut.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <assert.h>
#include <math.h>


GLuint VBO;

GLuint gWorldLocation;

GLuint IBO;

struct Matrix4f {
    float m[4][4];
};

static const char* VertShader = "#version 330\n\
layout (location = 0) in vec3 Position;\n\
uniform mat4 gWorld;\n\
out vec4 Color;\n\
void main()\n\
{gl_Position = gWorld * vec4(Position, 2.0);\n\
Color = vec4(clamp(Position, 0.0, 1.0), 1.0); }";

static const char* PixelShader = "#version 330\n\
out vec4 FragColor;\n\
in vec4 Color;\n\
void main()\n\
{//FragColor = vec4(0.2, 0.0, 0.8, 0.3);\n\
FragColor = Color;}";

//static const char* ArrShader = "#version 330\n\
//}";

void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    Scale += 0.002f;

    Matrix4f World;
    World.m[0][0] = 0.5*sinf(Scale); World.m[0][1] = 0.0f;        World.m[0][2] = 0.0f;        World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0f;        World.m[1][1] = 0.5*cosf(Scale); World.m[1][2] = 0.0f;        World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;        World.m[2][2] = sinf(Scale); World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;        World.m[3][2] = 0.0f;        World.m[3][3] = 1.0f;

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    AddShader(ShaderProgram, VertShader, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, PixelShader , GL_FRAGMENT_SHADER);
    
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    // lesson 1
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lesson_10");
    
    InitializeGlutCallbacks();

    glClearColor(0.8f, 0.3f, 0.0f, 0.0f);

    // lesson2
    
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    /////// lesson 6

    // создаем вершинный буфер
    glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    //создаем буфер индексов
    glm::vec3 VerticesI[4];
    VerticesI[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    VerticesI[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    VerticesI[2] = glm::vec3(0.0f, -1.0f, 0.0f);
    VerticesI[3] = glm::vec3(0.0f, 1.0f, 0.0f);

    unsigned int Indices[] = { 0, 3, 1,
                           1, 3, 2,
                           2, 3, 0,
                           0, 2, 1 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VerticesI), VerticesI, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    CompileShaders();

    glutMainLoop();

}
