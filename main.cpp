#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <glm/glm.hpp>

int WINDOW_HEIGHT = 900;
int WINDOW_WIDTH = 1200;
float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
const int x = 100;
const int y = 50;
const char WINDOW_NAME[] = "Rotating cube";
const GLclampf BACKGROUND_RED = 0.0f;
const GLclampf BACKGROUND_GREEN = 0.0f;
const GLclampf BACKGROUND_BLUE = 0.0f;
const GLclampf BACKGROUND_ALPHA = 0.0f;
char VERTEX_SHADER_FILENAME[] = "shader.vs";
char FRAGMENT_SHADER_FILENAME[] = "shader.fs";
unsigned int RANDOM_SEED = 42;
GLfloat FOV = 100.0;
GLfloat OneOverTanHalfFov = 1.0 / tanf(FOV * M_PI / 360);
GLfloat NEAR_Z = 1.0;
GLfloat FAR_Z = 100.0;

GLuint WVP;
GLuint cubeVBO;
GLuint cubeCBO;
GLuint cubeIBO;
GLuint shaderProgram;

GLfloat cube_vertex_data[][3] = {
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.5f},
};
GLuint cube_index_data[][3] = {
    // LEFT
    {1, 2, 3},
    {0, 2, 1},
    // BOTTOM
    {0, 1, 4},
    {1, 5, 4},
    // TOP
    {2, 6, 3},
    {3, 6, 7},
    // BACK
    {3, 5, 1},
    {3, 7, 5},
    // RIGHT
    {4, 5, 6},
    {5, 7, 6},
    // FRONT
    {0, 4, 2},
    {2, 4, 6},
};
GLfloat cube_color_data[][3] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}};

static GLfloat angle = 0.0f;
static GLfloat deltaAngle = 0.0f;
static GLfloat deltaDeltaAngle = 0.0001f;
static GLfloat scale = 1.0f;
static GLfloat deltaScale = 0.01f;
static glm::vec3 translationVector = glm::vec3(0.0f, 0.0f, 1.3f);
static GLfloat deltaTranslation = 0.1f;
static glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
static GLfloat deltaCameraPosition = 0.1f;
static GLfloat cameraAngle = 0.0f;
static GLfloat deltaCameraAngle = 0.1f;

void RenderCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeCBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    angle += deltaAngle;

    glm::mat4 rotationMatrix = glm::mat4({
        {cosf(angle), 0.0, -sinf(angle), 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {sinf(angle), 0.0, cosf(angle), 0.0},
        {0.0, 0.0, 0.0, 1.0},
    });
    glm::mat4 scaleMatrix = glm::mat4({
        {scale, 0.0, 0.0, 0.0},
        {0.0, scale, 0.0, 0.0},
        {0.0, 0.0, scale, 0.0},
        {0.0, 0.0, 0.0, 1.0},
    });
    glm::mat4 translationMatrix = glm::mat4({
        {1.0, 0.0, 0.0, translationVector.x},
        {0.0, 1.0, 0.0, translationVector.y},
        {0.0, 0.0, 1.0, translationVector.z},
        {0.0, 0.0, 0.0, 1.0},
    });
    glm::mat4 perspectiveProjectionMatrix = glm::mat4({
        {OneOverTanHalfFov / ASPECT_RATIO, 0.0, 0.0, 0.0},
        {0.0, OneOverTanHalfFov, 0.0, 0.0},
        {0.0, 0.0, (-NEAR_Z - FAR_Z) / (NEAR_Z - FAR_Z), 2 * FAR_Z * NEAR_Z / (NEAR_Z - FAR_Z)},
        {0.0, 0.0, 1.0, 0.0},
    });
    glm::mat4 viewRotationMatrix = glm::mat4({
        {cosf(cameraAngle), 0.0, -sinf(cameraAngle), 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {sinf(cameraAngle), 0.0, cosf(cameraAngle), 0.0},
        {0.0, 0.0, 0.0, 1.0},
    });
    glm::mat4 viewPositionMatrix = glm::mat4({
        {1.0, 0.0, 0.0, cameraPosition.x},
        {0.0, 1.0, 0.0, cameraPosition.y},
        {0.0, 0.0, 1.0, cameraPosition.z},
        {0.0, 0.0, 0.0, 1.0},
    });
    glm::mat4 finalMatrix = viewPositionMatrix * viewRotationMatrix * translationMatrix * rotationMatrix * scaleMatrix;
    glUniformMatrix4fv(WVP, 1, GL_FALSE, &finalMatrix[0][0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
    glDrawElements(GL_TRIANGLES, (sizeof(cube_index_data) / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay();
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
        printf("Impossible to open %s.\n", file);
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
    WVP = glGetUniformLocation(shaderProgram, "WVP");
    glUseProgram(shaderProgram);
}

void KeyboardCB(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 81:
    case 113:
        cameraAngle -= deltaCameraAngle;
        break;
    case 69:
    case 101:
        cameraAngle += deltaCameraAngle;
        break;
    default:
        break;
    }
}

void SpecialCB(int key, int x, int y)
{
    int modifier = glutGetModifiers();
    switch (key)
    {
    case GLUT_KEY_UP:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            translationVector.y += deltaTranslation;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            translationVector.z += deltaTranslation;
            break;
        default:
            cameraPosition.z += deltaCameraPosition;
            break;
        }
        break;
    case GLUT_KEY_DOWN:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            translationVector.y -= deltaTranslation;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            translationVector.z -= deltaTranslation;
            break;
        default:
            cameraPosition.z -= deltaCameraPosition;
            break;
        }
        break;
    case GLUT_KEY_LEFT:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            translationVector.x -= deltaTranslation;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            break;
        default:
            cameraPosition.x -= deltaCameraPosition;
            break;
        }
        break;
    case GLUT_KEY_RIGHT:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            translationVector.x += deltaTranslation;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            break;
        default:
            cameraPosition.x += deltaCameraPosition;
            break;
        }
        break;
    case GLUT_KEY_PAGE_UP:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            scale += deltaScale;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            break;
        default:
            deltaAngle += deltaDeltaAngle;
            break;
        }
        break;
    case GLUT_KEY_PAGE_DOWN:
        switch (modifier)
        {
        case GLUT_ACTIVE_SHIFT:
            scale -= deltaScale;
            break;
        case GLUT_ACTIVE_CTRL:
            break;
        case GLUT_ACTIVE_ALT:
            break;
        default:
            deltaAngle -= deltaDeltaAngle;
            break;
        }
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

    glGenBuffers(1, &cubeCBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeCBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_data), cube_color_data, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_data), cube_index_data, GL_STATIC_DRAW);
}

int main(int argc, char *argv[])
{
    srandom(RANDOM_SEED);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(x, y);

    glutCreateWindow(WINDOW_NAME);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);

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
    glEnable(GL_CULL_FACE);
    // glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glutMainLoop();

    return 0;
}