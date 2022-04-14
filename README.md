# Lab1_ECG
### The laboratory work №1 on engineering and computer graphics
# Step 1
## Open a window
#### Including libraries
```c++ 
#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/vec3.hpp>
```
Include all necessary libraries.

#### GLUT initialization
```c++ 
glutInit(&argc, argv);
```
This call initializes GLUT.
The first parameter is a pointer to the number of arguments on the command line. The second is a pointer to an array of arguments. Usually these values are taken from main(int argc, char *argv[]).

#### Setting window parameters
```c++ 
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
```
Here we configure some GLUT options. GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed) and the color buffer where most rendering ends up (i.e. the screen).
```c++ 
glutInitWindowSize(700, 700);
glutInitWindowPosition(100, 100);
```
These calls specify the window parameters and create it.

#### Creating a window
```c++ 
glutCreateWindow("Tutorial 2");
```
This command creates a window with a header that you specify as a parameter and returns the window HANDLER as an int number. This HANDLER is usually used for subsequent operations on this window, such as changing window parameters and closing the window.

#### Installation of functions responsible for drawing in the window
```c++ 
glClearColor(0.8f, 0.3f, 0.0f, 0.0f);
```
Specify the red, green, blue, and alpha values used when the color buffers are cleared.
```c++ 
glutDisplayFunc(RenderSceneCB);
```
The only parameter of this function is a pointer to the function that will be responsible for drawing in the window.
After the window into which graphical information will be displayed or rendered, it is necessary to associate with it procedures that will be responsible for displaying graphical information, monitor the size of the window, monitor keystrokes, etc. The very first and most necessary function is responsible for drawing. It will always be called by the operating system to draw (redraw) the contents of the window.
```c++
 glClear(GL_COLOR_BUFFER_BIT);
```
This function sets the color that will clear the screen. 
```c++
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```
Calling function binds that buffer object name to the target.
```c++
glutSwapBuffers();
```
This function asks GLUT to swap the background buffer and the frame buffer. In the next call, the render will occur in the current frame buffer, and the background buffer will be displayed.

#### Entering the main GLUT cycle
```c++ 
glutMainLoop();
```
The main GLUT loop launches the so-called GLUT heart, which provides the relationship between the operating system and those functions that are responsible for the window, receive information from input/output devices.

![image](https://user-images.githubusercontent.com/81005466/162557134-40b4ee80-f2ff-4491-8189-800147cf6a57.png)
# Step 2, 3
## Drawing point/triangle
```c++ 
glm::vec3 Vertices[1];
    Vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);
```
Initialize vertice to point
```c++ 
glm::vec3 Vertices[3]{
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
```
or vertices to triangle.
```c++ 
GLuint VBO;
glGenBuffers(1, &VBO);
```
Creating a "general type" buffer. The first argument defines the number of objects that you want to create, and the second is a reference to an array of GLuints type to store a pointer by which data will be stored. In the program, we created GLuint VBO as a global variable to store a pointer to the vertex buffer.
```c++
glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
```
The call takes the target name (same as what we used for binding), the size of the data in bytes, address of the array of vertices and a flag that indicates the usage pattern for this data.

```c++
glEnableVertexAttribArray(0);
```
Specifies the index of the generic vertex attribute to be enabled
```c++
glDisableVertexAttribArray(0);
```
or disabled.
```c++
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
```
This call tells the pipeline how to interpret the data inside the buffer. The first parameter specifies the index of the attribute. In our case we know that it is zero by default but when we start using shaders we will either need to explicitly set the index in the shader or query it. The second parameter is the number of components in the attribute (3 for X, Y and Z). The third parameter is the data type of each component. The next parameter indicates whether we want our attribute to be normalized before it is used in the pipeline. It our case we want the data to pass unchanged. The fifth parameter (called the 'stride') is the number of bytes between two instances of that attribute in the buffer. 
```c++
glDrawArrays(GL_POINTS, 0, 1); // for point
```
```c++
glDrawArrays(GL_TRIANGLES, 0, 3); // for triangle
```
This is an ordinal rendering function. Accepts the data type, index of the first vertex and their number. 
```c++
glColor3f(0,0,0);
```
A function that sets the current drawing color of both the primitive and individual parts.

![image](https://user-images.githubusercontent.com/81005466/162557103-1483a21c-8db9-4c46-816d-19e279f2f3e7.png)
![image](https://user-images.githubusercontent.com/81005466/162557062-c41f4ec8-07fc-463b-bcdd-60440c5c217a.png)
