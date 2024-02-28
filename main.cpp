#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <vector>
#include <cstdio>
const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;
const int x = 100;
const int y = 200;
const char WINDOW_NAME[] = "Rotating cube";
const GLclampf BACKGROUND_RED = 0.5f;
const GLclampf BACKGROUND_GREEN = 0.5f;
const GLclampf BACKGROUND_BLUE = 0.0f;
const GLclampf BACKGROUND_ALPHA = 0.0f;

GLuint VAO;
GLuint VBO;

GLfloat cube_vertex_data[] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    // -1.0f,
    // -1.0f,
    // -1.0f,
    // -1.0f,
    // -1.0f,
    // 1.0f,
    // -1.0f,
    // 1.0f,
    // -1.0f,
    // -1.0f,
    // 1.0f,
    // 1.0f,
    // 1.0f,
    // -1.0f,
    // -1.0f,
    // 1.0f,
    // -1.0f,
    // 1.0f,
    // 1.0f,
    // 1.0f,
    // -1.0f,
    // 1.0f,
    // 1.0f,
    // 1.0f,
};
unsigned int cube_index_data[] = {
    1, 2, 3,
    2, 3, 4,
    3, 4, 8,
    3, 7, 8,
    5, 6, 7,
    6, 7, 8,
    1, 2, 5,
    2, 5, 6,
    2, 4, 8,
    2, 6, 8,
    1, 3, 5,
    3, 5, 7};

void RenderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    // glDrawElements(GL_TRIANGLES, cube_index_data.size(), GL_UNSIGNED_INT, (void *)0);
    glDrawArrays(GL_TRIANGLES, 0, 2);
    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void KeyboardCB(unsigned char key, int x, int y)
{
}

void SpecialCB(int key, int x, int y)
{
    const int positionChangeSpeed = 10;
    const int darkMatterConst = 37;
    switch (key)
    {
    case GLUT_KEY_UP:
        glutPositionWindow(glutGet(GLUT_WINDOW_X), glutGet(GLUT_WINDOW_Y) - positionChangeSpeed - darkMatterConst);
        break;
    case GLUT_KEY_DOWN:
        glutPositionWindow(glutGet(GLUT_WINDOW_X), glutGet(GLUT_WINDOW_Y) + positionChangeSpeed - darkMatterConst);
        break;
    case GLUT_KEY_LEFT:
        glutPositionWindow(glutGet(GLUT_WINDOW_X) - positionChangeSpeed, glutGet(GLUT_WINDOW_Y) - darkMatterConst);
        break;
    case GLUT_KEY_RIGHT:
        glutPositionWindow(glutGet(GLUT_WINDOW_X) + positionChangeSpeed, glutGet(GLUT_WINDOW_Y) - darkMatterConst);
        break;
    default:
        break;
    }
}

void CreateVertexArrays()
{
    glGenBuffers(1, &VAO);
    // glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_data), &cube_vertex_data[0], GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_index_data.size() * sizeof(unsigned int), &cube_index_data[0], GL_STATIC_DRAW);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(x, y);

    glutCreateWindow(WINDOW_NAME);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutPostRedisplay();

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA);

    CreateVertexArrays();

    glutDisplayFunc(RenderCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialCB);

    glutMainLoop();

    return 0;
}