#include <GL/freeglut_std.h>

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;
const int x = 100;
const int y = 200;
const char WINDOW_NAME[] = "Rotating cube"; 

void RenderCB() {

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(x, y);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_ALPHA);

    glutCreateWindow(WINDOW_NAME);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutPostRedisplay();

    glutDisplayFunc(RenderCB);

    glutMainLoop();

    return 0;
}