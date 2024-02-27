#include <GL/freeglut_std.h>

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;
const int x = 100;
const int y = 200;
const char WINDOW_NAME[] = "Rotating cube";
const GLfloat BACKGROUND_RED = 0.5f;
const GLfloat BACKGROUND_GREEN = 0.5f;
const GLfloat BACKGROUND_BLUE = 0.0f;

void RenderCB()
{

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

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(x, y);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);

    glutCreateWindow(WINDOW_NAME);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutSetColor(1, BACKGROUND_BLUE, BACKGROUND_GREEN, BACKGROUND_BLUE);
    glutPostRedisplay();

    glutDisplayFunc(RenderCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialCB);

    glutMainLoop();

    return 0;
}