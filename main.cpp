#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <vector>
#include <cstdio>

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;
const int x = 100;
const int y = 200;
const char WINDOW_NAME[] = "Rotating cube";
const GLclampf BACKGROUND_RED = 0.1f;
const GLclampf BACKGROUND_GREEN = 0.1f;
const GLclampf BACKGROUND_BLUE = 0.0f;
const GLclampf BACKGROUND_ALPHA = 0.0f;
char VERTEX_SHADER_FILENAME[] = "shader.vs";
char FRAGMENT_SHADER_FILENAME[] = "shader.fs";
GLuint transformMatrix;

GLuint cubeVertexVBO;
GLuint cubeIndexVBO;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLchar *vertexShaderSource;
GLchar *fragmentShaderSource;

GLfloat cube_vertex_data[][3] = {
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    // {-0.5f, -0.5f, 0.0f},
    // {-0.5f, -0.5f, 0.5f},
    // {-0.5f, 0.5f, 0.0f},
    // {0.5f, 0.5f, 0.5f},
    // {0.5f, -0.5f, 0.0f},
    // {0.5f, -0.5f, 0.5f},
    // {0.5f, 0.5f, 0.0f},
    // {0.5f, 0.5f, 0.5f},
};
unsigned int cube_index_data[][3] = {
    {0, 1, 2},
    {1, 2, 3},
    {2, 3, 7},
    {2, 6, 7},
    {4, 5, 6},
    {5, 6, 7},
    {0, 1, 4},
    {1, 3, 5},
    {1, 3, 7},
    {1, 5, 7},
    {0, 2, 4},
    {2, 4, 6}
    };

void RenderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, (sizeof(cube_vertex_data) / 3 / sizeof(GLfloat)));
    GLfloat scale = 0.1;
    GLfloat matrix[4][4] = {
        {scale, 0.0, 0.0, 0.0},
        {0.0, scale, 0.0, 0.0},
        {0.0, 0.0, scale, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    transformMatrix = glGetUniformLocation(shaderProgram, "transformMatrix");
    glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, &matrix[0][0]);
    glDrawElements(GL_TRIANGLES, (sizeof(cube_index_data) / 3 / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

char *filetobuf(char *file)
{
    FILE *filePointer;
    long length;
    char *buf;

    filePointer = fopen(file, "rb");
    if (!filePointer)
        return NULL;

    fseek(filePointer, 0, SEEK_END);
    length = ftell(filePointer);
    buf = (char *)malloc(length + 1);
    fseek(filePointer, 0, SEEK_SET);
    fread(buf, length, 1, filePointer);
    fclose(filePointer);
    buf[length] = 0;

    return buf;
}

void checkShaderCompilation(GLuint &shader)
{
    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar *shaderInfoLog = (char *)malloc(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, shaderInfoLog);

        // Надо кидать ошибку или как-то ее обрабатывать

        free(shaderInfoLog);
        exit(-1);
    }
}

void CompileShaders()
{
    vertexShaderSource = filetobuf(VERTEX_SHADER_FILENAME);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar **)&vertexShaderSource, 0);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader);
    free(vertexShaderSource);

    fragmentShaderSource = filetobuf(FRAGMENT_SHADER_FILENAME);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **)&vertexShaderSource, 0);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader);
    free(fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
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
    glGenBuffers(1, &cubeVertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_data), cube_vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_data), cube_index_data, GL_STATIC_DRAW);
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