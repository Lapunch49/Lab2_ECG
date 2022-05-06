#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <assert.h>
#include <math.h>
#include <cmath>

const double PI = acos(-1.0); // need to converå from degree to radian

GLuint VBO;

GLuint gWorldLocation;

GLuint IBO;

struct { // for perspective projection
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
} m_persProj;

struct { // for determining the location of the camera
    glm::vec3 Pos;
    glm::vec3 Target;
    glm::vec3 Up;
} m_camera;

class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ) // setting the scale parameters
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z) // setting the  parameters of moving
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ) // setting the  parameters of rotation
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }
    void InitScaleTransform(glm::mat4& ScaleTrans) const  { // setting the scale matrix
        glm::mat4  World;
        World[0][0] = abs(sinf(m_scale.x)); World[0][1] = 0.0f;        World[0][2] = 0.0f;        World[0][3] = 0.0f;
        World[1][0] = 0.0f;        World[1][1] = abs(sinf(m_scale.y)); World[1][2] = 0.0f;        World[1][3] = 0.0f;
        World[2][0] = 0.0f;        World[2][1] = 0.0f;                 World[2][2] = 1.0f;        World[2][3] = 0.0f;
        World[3][0] = 0.0f;        World[3][1] = 0.0f;                 World[3][2] = 0.0f;        World[3][3] = 1.0f;
        ScaleTrans = World;
    }
    void InitRotateTransform(glm::mat4 & RotateTrans) const  { // initializing the rotation matrixs

        const float x = m_rotateInfo.x * PI / 180.0f;
        const float y = m_rotateInfo.y * PI / 180.0f;
        const float z = m_rotateInfo.z * PI / 180.0f;

        glm::mat4  RotateX;
        glm::mat4  RotateY;
        glm::mat4  RotateZ;

        // matrix for rotation for the YZ (around the X axis) 
        RotateX[0][0] = 1.0f; RotateX[0][1] = 0.0f; RotateX[0][2] = 0.0f; RotateX[0][3] = 0.0f;
        RotateX[1][0] = 0.0f; RotateX[1][1] = cosf(x); RotateX[1][2] = -sinf(x); RotateX[1][3] = 0.0f;
        RotateX[2][0] = 0.0f; RotateX[2][1] = sinf(x); RotateX[2][2] = cosf(x); RotateX[2][3] = 0.0f;
        RotateX[3][0] = 0.0f; RotateX[3][1] = 0.0f; RotateX[3][2] = 0.0f; RotateX[3][3] = 1.0f;
        // matrix for rotation for the XZ (around the Y axis) 
        RotateY[0][0] = cosf(y); RotateY[0][1] = 0.0f; RotateY[0][2] = -sinf(y); RotateY[0][3] = 0.0f;
        RotateY[1][0] = 0.0f; RotateY[1][1] = 1.0f; RotateY[1][2] = 0.0f; RotateY[1][3] = 0.0f;
        RotateY[2][0] = sinf(y); RotateY[2][1] = 0.0f; RotateY[2][2] = cosf(y); RotateY[2][3] = 0.0f;
        RotateY[3][0] = 0.0f; RotateY[3][1] = 0.0f; RotateY[3][2] = 0.0f; RotateY[3][3] = 1.0f;
        // matrix for rotation for the XY (around the Z axis) 
        RotateZ[0][0] = cosf(z); RotateZ[0][1] = -sinf(z); RotateZ[0][2] = 0.0f; RotateZ[0][3] = 0.0f;
        RotateZ[1][0] = sinf(z); RotateZ[1][1] = cosf(z); RotateZ[1][2] = 0.0f; RotateZ[1][3] = 0.0f;
        RotateZ[2][0] = 0.0f; RotateZ[2][1] = 0.0f; RotateZ[2][2] = 1.0f; RotateZ[2][3] = 0.0f;
        RotateZ[3][0] = 0.0f; RotateZ[3][1] = 0.0f; RotateZ[3][2] = 0.0f; RotateZ[3][3] = 1.0f;

        RotateTrans = RotateX * RotateY * RotateZ;
    }

    void InitTranslationTransform(glm::mat4 & TranslationTrans) const { // setting the moving parameters
        glm::mat4  Move;

        Move[0][0] = 1.0f; Move[0][1] = 0.0f; Move[0][2] = 0.0f; Move[0][3] = sinf(m_worldPos.x);
        Move[1][0] = 0.0f; Move[1][1] = 1.0f; Move[1][2] = 0.0f; Move[1][3] = sinf(m_worldPos.x);
        Move[2][0] = 0.0f; Move[2][1] = 0.0f; Move[2][2] = 1.0f; Move[2][3] = sinf(m_worldPos.x);
        Move[3][0] = 0.0f; Move[3][1] = 0.0f; Move[3][2] = 0.0f; Move[3][3] = 1.0f;

        TranslationTrans = Move;
    }

    glm::mat4 InitTranslationTransform(float x, float y, float z) const { // this 2nd function is required to Camera Transformation
        glm::mat4  Move;

        Move[0][0] = 1.0f; Move[0][1] = 0.0f; Move[0][2] = 0.0f; Move[0][3] = x;
        Move[1][0] = 0.0f; Move[1][1] = 1.0f; Move[1][2] = 0.0f; Move[1][3] = y;
        Move[2][0] = 0.0f; Move[2][1] = 0.0f; Move[2][2] = 1.0f; Move[2][3] = z;
        Move[3][0] = 0.0f; Move[3][1] = 0.0f; Move[3][2] = 0.0f; Move[3][3] = 1.0f;

        return Move;
    }

    void InitPerspectiveProj(glm::mat4& m) const //  initializing the matrix for perspective projection
    {
        const float ar = m_persProj.Width / m_persProj.Height;
        const float zNear = m_persProj.zNear;
        const float zFar = m_persProj.zFar;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = tanf((m_persProj.FOV / 2.0) * PI / 180.0f);

        m[0][0] = 1.0f / (tanHalfFOV * ar);
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = 1.0f / tanHalfFOV;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = (-zNear - zFar) / zRange;
        m[2][3] = 2.0f * zFar * zNear / zRange;

        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 1.0f;
        m[3][3] = 0.0f;
    }
    void SetPerspectiveProj(float FOV, float width, float height, float zFar, float zNear) { // setting the parameters for perspective projection
        m_persProj.FOV = FOV;
        m_persProj.Width = width;
        m_persProj.Height = height;
        m_persProj.zFar = zFar;
        m_persProj.zNear = zNear;
    }
    void Normalize(glm::vec3& Vec) { // to normalize vectors
        const float Length = sqrtf(Vec.x * Vec.x + Vec.y* Vec.y + Vec.z * Vec.z);
        Vec.x /= Length;
        Vec.y /= Length;
        Vec.z /= Length;
    }
    glm::vec3 Cross(glm::vec3& v1, glm::vec3&v2) const // to find a vector perpendicular to these two
    {
        const float _x = v1.y * v2.z - v1.z * v2.y;
        const float _y = v1.z * v2.x - v1.x * v2.z;
        const float _z = v1.x * v2.y - v1.y * v2.x;

        return glm::vec3(_x, _y, _z);
    }
    glm::mat4 InitCameraTransform(glm::vec3& Target, const glm::vec3& Up) // setting camera transformation parameters
    {
        glm::vec3 N = Target;
        Normalize(N);
        glm::vec3 U = Up;
        Normalize(U);
        U = Cross(U, Target);
        glm::vec3 V = Cross(N, U);

        glm::mat4 m;
        m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
        m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
        m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
        return m;
    }
    void SetCamera(glm::vec3 CameraPos, glm::vec3 CameraTarget, glm::vec3 CameraUp) {
        m_camera.Pos = CameraPos;
        m_camera.Target = CameraTarget;
        m_camera.Up = CameraUp;
    }
    const glm::mat4 * GetTrans();
private:
    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4  m_transformation;
};

const glm::mat4* Pipeline::GetTrans() // unite all the transformation
{
    glm::mat4 ScaleTrans, RotateTrans,
        TranslationTrans, CameraTranslationTrans,
        CameraRotateTrans, PersProjTrans;

    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);
    CameraTranslationTrans = InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    CameraRotateTrans = InitCameraTransform(m_camera.Target, m_camera.Up);

    InitPerspectiveProj(PersProjTrans);

    m_transformation = //PersProjTrans * 
        CameraRotateTrans *
        CameraTranslationTrans * 
        TranslationTrans *
        RotateTrans * ScaleTrans;

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

    p.SetPerspectiveProj(60.0f, GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT, 1.0f, 100.0f);

    glm::vec3 CameraPos(1.0f, 1.0f, 1.0f);
    glm::vec3 CameraTarget(0.45f, 0.0f, 1.0f);
    glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);

    p.SetCamera(CameraPos, CameraTarget, CameraUp);

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
    // initializing the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // setting the size, position and name of window
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lesson_13");
    
    InitializeGlutCallbacks();

    // clearing the window with black color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // creating a vertex buffer
    glm::vec3 Vertices[4];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(0.0f, 0.0f, 2.0f);//(1.0f, -1.0f, 0.0f);
    Vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);//(0.0f, 0.0f, 1.0f);
    Vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);//(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // creating an index buffer
    unsigned int Indices[] = { 0, 3, 1,
                           1, 3, 2,
                           2, 3, 0,
                           0, 2, 1 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    CompileShaders();

    // entering the main loop
    glutMainLoop();

}
