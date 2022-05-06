# Lab2_ECG
### The laboratory work №2 on engineering and computer graphics
Lessons 6-8, 11-13 from https://triplepointfive.github.io/ogltutor/

We create the class Pipeline to abstract the details of getting all the transformation required for one single object combined.
# Lesson 6: Translation Transformation
```c++ 
class Pipeline
{
public:
...
void WorldPos(float x, float y, float z) // setting the  parameters of moving
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }
void InitTranslationTransform(glm::mat4 & TranslationTrans) const {
        glm::mat4  Move;

        Move[0][0] = 1.0f; Move[0][1] = 0.0f; Move[0][2] = 0.0f; Move[0][3] = sinf(m_worldPos.x);
        Move[1][0] = 0.0f; Move[1][1] = 1.0f; Move[1][2] = 0.0f; Move[1][3] = sinf(m_worldPos.x);
        Move[2][0] = 0.0f; Move[2][1] = 0.0f; Move[2][2] = 1.0f; Move[2][3] = sinf(m_worldPos.x);
        Move[3][0] = 0.0f; Move[3][1] = 0.0f; Move[3][2] = 0.0f; Move[3][3] = 1.0f;

        TranslationTrans = Move;
    }
...
private:
    glm::vec3 m_worldPos;
```
In the render function we prepare a 4x4 matrix and populate it. We set v1, v2, v3 to the result of the sinus function. This will translate the X, Y and Z coordinate by a value that swings nicely between -1 and 1.

# Lesson 7: Rotation Transformation
```c++ 
class Pipeline
{
public:
...
void Rotate(float RotateX, float RotateY, float RotateZ) // setting the  parameters of rotation
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
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
...
private:
    glm::vec3 m_rotateInfo;
```
Given an angle and a point we want to rotate the point around one of the axis. We will always change two out of the trio X, Y and Z and leave the third component unchanged. This means that the path will lie on one of the three major planes: XY (when turning around Z), Y Z (when turning around X) and XZ (when turning around Y). 

# Lesson 8: Scaling Transformation
```c++ 
class Pipeline
{
public:
...
void Scale(float ScaleX, float ScaleY, float ScaleZ) // setting the scale parameters
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }
void InitScaleTransform(glm::mat4& ScaleTrans) const  { // initializing the scale matrix
        glm::mat4  World;
        World[0][0] = abs(sinf(m_scale.x)); World[0][1] = 0.0f;        World[0][2] = 0.0f;        World[0][3] = 0.0f;
        World[1][0] = 0.0f;        World[1][1] = abs(sinf(m_scale.y)); World[1][2] = 0.0f;        World[1][3] = 0.0f;
        World[2][0] = 0.0f;        World[2][1] = 0.0f;                 World[2][2] = 1.0f;        World[2][3] = 0.0f;
        World[3][0] = 0.0f;        World[3][1] = 0.0f;                 World[3][2] = 0.0f;        World[3][3] = 1.0f;
        ScaleTrans = World;
    } 
...
private:
    glm::vec3 m_scale;
```
Developing the transformation matrix is very simple. We start with the identity matrix and remember that the reason that multiplying it by a vector leave the vector unchanged is that each of the '1's in the diagonal is multiplied by one of the components in turn. None of the components can affect the other. Therefore, replacing any one of that '1's with another value will cause the object to increase on that axis if the other value is larger than 1 or decrease on that axis if the other value is smaller then one.

# Lesson 11: Concatenating Transformations
```c++ 
class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }
...
const glm::mat4 * GetTrans();
private:
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
```
This function initializes some separate matrices as the transformations that match the current configuration. It multiplies them one by one and returns the final product. The order is hard coded. It uses private methods to generate the different transformations.
```c++ 
void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    Scale += 0.001f;

    Pipeline p;
    p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
    ...
}
```
These are the changes to the render function. We allocate a pipeline object, configure it and send the resulting transformation down to the shader.  
# Lesson 12: Perspective Projection
```c++ 
struct { // for perspective projection
    float FOV;
    float Width;
    float Height;
    float zNear;
    float zFar;
} m_persProj;

class Pipeline
{
public:
...
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
...
};
void RenderSceneCB()
{
...
    p.SetPerspectiveProj(60.0f, GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT, 1.0f, 100.0f);
...
}
```
We have generated the transformation that satisfies the  requirement the projection from the 3D world on a 2D plane while maintaining the appearance of depth and we have an additional requirement we "piggyback"ed on it which is to make life easier for the clipper by representing the projected coordinates in a normalized space of -1 to +1. This means the clipper can do its work without having knowledge of the screen dimension and the location of the near and far planes.

The perspective projection tranformation will require us to supply 4 parameters:
-The aspect ratio - the ratio between the width and the height of the rectangular area which will be the target of projection.

-The vertical field of view: the vertical angle of the camera through which we are looking at the world.

-The location of the near Z plane. This allows us to clip objects that are too close to the camera.

-The location of the far Z plane. This allows us to clip objects that are too distant from the camera.

A structure called m_persProj was added to the Pipeline class that holds the perspective projection configurations.
In the render function we set the projection parameters.

# Lesson 13: Camera Space
```c++ 
struct { // for determining the location of the camera
    glm::vec3 Pos;
    glm::vec3 Target;
    glm::vec3 Up;
} m_camera;
class Pipeline
{
public:
...
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
...
}
void RenderSceneCB()
{
...
    glm::vec3 CameraPos(1.0f, 1.0f, 1.0f);
    glm::vec3 CameraTarget(0.45f, 0.0f, 1.0f);
    glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);

    p.SetCamera(CameraPos, CameraTarget, CameraUp);
...
}
```
The camera is defined by the following vectors:

-N - The vector from the camera to its target. Also known as the 'look at' vector in some 3D literature. This vector corresponds to the Z axe.

-V - When standing upright this is the vector from your head to the sky. If you are writing a flight simulator and the plane is reversed that vector may very well point to the ground. This vector corresponds to the Y axe.

-U - This vector points from the camera to its "right" side". It corresponds to the X axe.

In order to transform a position in world space to the camera space defined by the UVN vectors we need to perform a dot product operation between the position and the UVN vectors.

The Pipeline has a new method glm::vec3 Cross() to calculate the cross product between two Vector objects. A cross product between two vectors produces a vector which is perpendicular to the plane defined by the vectors. 

void Normalize() - to generate the UVN matrix we will need to make the vectors unit length. This operation is formally known as 'vector normalization' is executed by dividing each vector component by the vector length. 

glm::mat4 InitCameraTransform() - this function generates the camera transformation matrix that will be used later by the pipeline class. The U,V and N vectors are calculated and set into the matrix in rows. Since the vertex position is going to be multiplied on the right side (as a column vector) this means a dot product between U,V and N and the position. This generates the 3 scalar projections magnitude values that become the XYZ values of the position in screen space.

![lab_ecg2](https://user-images.githubusercontent.com/81005466/167222494-f08bb4ff-feb4-48eb-9d9a-7243182487e2.gif)
