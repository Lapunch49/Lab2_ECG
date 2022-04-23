#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <assert.h>
#include <math.h>
#include <cmath>

#define _USE_MATH_DEFINES
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

const double PI = acos(-1.0); 

GLuint VBO;

GLuint gWorldLocation;

GLuint IBO;

struct {
    float FOV;
    float Width;
    float Height;
    float Z_near;
    float Z_far;
} proj;

class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }
    void InitScaleTransform(glm::mat4& ScaleTrans) const  {
        glm::mat4  World;
        World[0][0] = abs(sinf(m_scale.x)); World[0][1] = 0.0f;        World[0][2] = 0.0f;        World[0][3] = 0.0f;
        World[1][0] = 0.0f;        World[1][1] = abs(sinf(m_scale.y)); World[1][2] = 0.0f;        World[1][3] = 0.0f;
        World[2][0] = 0.0f;        World[2][1] = 0.0f;                 World[2][2] = 1.0f;           World[2][3] = 0.0f;
        World[3][0] = 0.0f;        World[3][1] = 0.0f;                 World[3][2] = 0.0f;        World[3][3] = 1.0f;
        ScaleTrans = World;
    }
    void InitRotateTransform(glm::mat4 & RotateTrans) const  {

        const float x = m_rotateInfo.x * PI / 180.0f;
        const float y = m_rotateInfo.y * PI / 180.0f;
        const float z = m_rotateInfo.z * PI / 180.0f;

        glm::mat4  RotateX;
        glm::mat4  RotateY;
        glm::mat4  RotateZ;

        RotateX[0][0] = 1.0f; RotateX[0][1] = 0.0f; RotateX[0][2] = 0.0f; RotateX[0][3] = 0.0f;
        RotateX[1][0] = 0.0f; RotateX[1][1] = cosf(x); RotateX[1][2] = -sinf(x); RotateX[1][3] = 0.0f;
        RotateX[2][0] = 0.0f; RotateX[2][1] = sinf(x); RotateX[2][2] = cosf(x); RotateX[2][3] = 0.0f;
        RotateX[3][0] = 0.0f; RotateX[3][1] = 0.0f; RotateX[3][2] = 0.0f; RotateX[3][3] = 1.0f;

        RotateY[0][0] = cosf(y); RotateY[0][1] = 0.0f; RotateY[0][2] = -sinf(y); RotateY[0][3] = 0.0f;
        RotateY[1][0] = 0.0f; RotateY[1][1] = 1.0f; RotateY[1][2] = 0.0f; RotateY[1][3] = 0.0f;
        RotateY[2][0] = sinf(y); RotateY[2][1] = 0.0f; RotateY[2][2] = cosf(y); RotateY[2][3] = 0.0f;
        RotateY[3][0] = 0.0f; RotateY[3][1] = 0.0f; RotateY[3][2] = 0.0f; RotateY[3][3] = 1.0f;

        RotateZ[0][0] = cosf(z); RotateZ[0][1] = -sinf(z); RotateZ[0][2] = 0.0f; RotateZ[0][3] = 0.0f;
        RotateZ[1][0] = sinf(z); RotateZ[1][1] = cosf(z); RotateZ[1][2] = 0.0f; RotateZ[1][3] = 0.0f;
        RotateZ[2][0] = 0.0f; RotateZ[2][1] = 0.0f; RotateZ[2][2] = 1.0f; RotateZ[2][3] = 0.0f;
        RotateZ[3][0] = 0.0f; RotateZ[3][1] = 0.0f; RotateZ[3][2] = 0.0f; RotateZ[3][3] = 1.0f;

        RotateTrans = RotateX * RotateY * RotateZ;
    }

    void InitTranslationTransform(glm::mat4 & TranslationTrans) const {
        glm::mat4  Move;

        Move[0][0] = 1.0f; Move[0][1] = 0.0f; Move[0][2] = 0.0f; Move[0][3] = sinf(m_worldPos.x);
        Move[1][0] = 0.0f; Move[1][1] = 1.0f; Move[1][2] = 0.0f; Move[1][3] = sinf(m_worldPos.x);
        Move[2][0] = 0.0f; Move[2][1] = 0.0f; Move[2][2] = 1.0f; Move[2][3] = sinf(m_worldPos.x);
        Move[3][0] = 0.0f; Move[3][1] = 0.0f; Move[3][2] = 0.0f; Move[3][3] = 1.0f;

        TranslationTrans = Move;
    }

    void InitializeProjection(glm::mat4 & matr) const {
        const float k = proj.Width / proj.Height;
        const float Z_range = proj.Z_near - proj.Z_far;
        const float tanHalfFOV = tanf((proj.FOV / 2.0f) * PI / 180.0f);

        matr[0][0] = 1.0f / (tanf((proj.FOV / 2.0f) * PI / 180.0f) * k); matr[0][1] = 0.0f; matr[0][2] = 0.0f; matr[0][3] = 0.0f;
        matr[1][0] = 0.0f; matr[1][1] = 1.0f; matr[1][2] = 0.0f; matr[1][3] = 0.0f;
        matr[2][0] = 0.0f; matr[2][1] = 0.0f; matr[2][2] = (-proj.Z_near - proj.Z_far) / Z_range; matr[2][3] = 2.0f * proj.Z_far * proj.Z_near / Z_range;
        matr[3][0] = 0.0f; matr[3][1] = 0.0f; matr[3][2] = 1.0f; matr[3][3] = 0.0f;
    }
    void SetPerspectiveProj(float FOV, float width, float height, float zFar, float zNear) {
        proj.FOV = FOV;
        proj.Width = width;
        proj.Height = height;
        proj.Z_far = zFar;
        proj.Z_near = zNear;
    }

    const glm::mat4 * GetTrans();
private:
    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4  m_transformation;
};

const glm::mat4 * Pipeline::GetTrans()
{
    glm::mat4  ScaleTrans, RotateTrans, TranslationTrans, ProjMatr;

    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);
    //InitializeProjection(ProjMatr);

    //m_transformation = ProjMatr * TranslationTrans * RotateTrans * ScaleTrans;

    m_transformation = TranslationTrans * RotateTrans * ScaleTrans;

    return &m_transformation;
}


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
FragColor = Color;\n\
}";

void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    Scale += 0.001f;

    Pipeline p;
    p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);


    p.SetPerspectiveProj(30.0f, GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT, 1.0f, 1000.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

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
    glutCreateWindow("Lesson_11");
    
    InitializeGlutCallbacks();

    glClearColor(0.8f, 0.3f, 0.0f, 0.0f);

    // lesson2
    
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // ������� ��������� �����
    glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    //������� ����� ��������
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
