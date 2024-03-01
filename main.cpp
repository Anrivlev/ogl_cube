#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

GLuint cubeVBO;
GLuint cubeIBO;

GLuint shaderProgram;

GLfloat cube_vertex_data[][3] = {
    {-0.5f, -0.5f, 0.0f},
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.0f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, 0.0f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.0f},
    {0.5f, 0.5f, 0.5f},
};
GLuint cube_index_data[][3] = {
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
    {2, 4, 6}};

void RenderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLfloat scale = 1.5;
    GLfloat matrix[4][4] = {
        {scale, 0.0, 0.0, 0.0},
        {0.0, scale, 0.0, 0.0},
        {0.0, 0.0, scale, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    };
    glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, &matrix[0][0]);
    // glDrawArrays(GL_TRIANGLES, 0, (sizeof(cube_vertex_data) / 3 / sizeof(GLfloat)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
    glDrawElements(GL_TRIANGLES, (sizeof(cube_index_data) / sizeof(GLuint)), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
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
        glGetShaderInfoLog(shader, maxLength, &maxLength, shaderInfoLog);

        // Надо кидать ошибку или как-то ее обрабатывать

        free(shaderInfoLog);
        exit(-1);
    }
}

void AddShader(GLuint &shader, char *file, GLenum shaderType)
{
    shader = glCreateShader(shaderType);
    std::string shaderSource;
    std::ifstream shaderStream(file, std::ios::in);
    if (shaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderSource = sstr.str();
        shaderStream.close();
    }
    else
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file);
        getchar();
        exit(0);
    }

    char const *shaderSourcePointer = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourcePointer, 0);
    glCompileShader(shader);
    checkShaderCompilation(shader);
}

void CompileShaders()
{
    GLuint vertexShader;
    GLuint fragmentShader;
    AddShader(vertexShader, VERTEX_SHADER_FILENAME, GL_VERTEX_SHADER);
    AddShader(fragmentShader, FRAGMENT_SHADER_FILENAME, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    transformMatrix = glGetUniformLocation(shaderProgram, "transformMatrix");
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

void CreateBuffers()
{
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_data), cube_vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
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

    glutDisplayFunc(RenderCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialCB);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA);

    CreateBuffers();
    CompileShaders();

    glutMainLoop();

    return 0;
}