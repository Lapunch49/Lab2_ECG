#include <iostream>
#include <GL\glew.h>
//#include <GL/glut.h>
#include <GL\freeglut.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

GLuint VBO;

void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //glDrawArrays(GL_POINTS, 0, 1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    // lesson 1
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 2");
    

    glClearColor(0.8f, 0.3f, 0.0f, 0.0f);

    // lesson2
    
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    /*glm::vec3 Vertices[1];
    Vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);*/

    glm::vec3 Vertices[3]{
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glutDisplayFunc(RenderSceneCB);
    glutMainLoop();

    glDrawElements(GL_POINTS, sizeof(Vertices), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0); //отключать каждый атрибут вершины, как только отпадает необходимость в нем

}















//#include <iostream>
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//
//GLuint VBO;
//
//void RenderSceneCB() {
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glDrawArrays(GL_POINTS, 0, 3);
//	glutSwapBuffers();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//
//	glutInitWindowSize(700, 400);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("Tutorial 01");
//
//	glutDisplayFunc(RenderSceneCB);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClearColor(253.0 / 255.0f, 106.0 / 255.0f, 2.0 / 255.0f, 0.0f);
//
//	glm::vec3 Vertices[1];
//	Vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);
//
//	GLenum res = glewInit();
//	if (res != GLEW_OK)
//	{
//		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//		return 1;
//	}
//
//	glutMainLoop();
//	glDisableVertexAttribArray(0);
//}
