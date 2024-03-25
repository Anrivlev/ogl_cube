#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static float WINDOW_HEIGHT = 300.0;
static float WINDOW_WIDTH = 400.0;
static float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
static const int x = 100;
static const int y = 50;
static const char WINDOW_NAME[] = "Rotating cube";
static const GLclampf BACKGROUND_RED = 0.0f;
static const GLclampf BACKGROUND_GREEN = 0.0f;
static const GLclampf BACKGROUND_BLUE = 0.0f;
static const GLclampf BACKGROUND_ALPHA = 0.0f;
static char VERTEX_SHADER_FILENAME[] = "shader.vs";
static char FRAGMENT_SHADER_FILENAME[] = "shader.fs";
static unsigned int RANDOM_SEED = 42;
static GLfloat FOV = 120.0;
static GLfloat NEAR_Z = 0.1;
static GLfloat FAR_Z = 10.0;

static GLuint WVP;
static GLuint cubeVBO;
static GLuint cubeCBO;
static GLuint cubeIBO;
static GLuint texture;
static int textureWidth, textureHeight, textureNrChannels;
unsigned char *textureData;
static GLuint shaderProgram;

static GLfloat cube_vertex_data[][3] = {
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.5f},
};

static GLuint cube_index_data[][3] = {
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

static GLfloat cube_color_data[][3] = {
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    // {0.6f, 0.6f, 0.0f},
    {0.2f, 0.2f, 0.2f},
    {0.2f, 0.2f, 0.8f},
    {0.2f, 0.8f, 0.2f},
    {0.2f, 0.8f, 0.8f},
    {0.8f, 0.2f, 0.2f},
    {0.8f, 0.2f, 0.8f},
    {0.8f, 0.8f, 0.2f},
    {0.8f, 0.8f, 0.8f}};

static GLfloat cube_texture_data[][2] = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
};

static GLfloat angle = 0.0f;
static GLfloat deltaAngle = 0.0f;
static GLfloat deltaDeltaAngle = 0.0001f;
static GLfloat scale = 1.0f;
static GLfloat deltaScale = 0.01f;
static glm::vec3 translationVector = glm::vec3(0.0f, 0.0f, -1.5f);
static GLfloat deltaTranslation = 0.1f;
static glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
static GLfloat deltaCameraPosition = 0.1f;
static glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
static glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

void RenderCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeCBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texture);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    angle += deltaAngle;

    glm::mat4 rotateM = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
    glm::mat4 translateM = glm::translate(glm::mat4(1.0f), translationVector);
    // GLfloat OneOverTanHalfFov = 1.0 / tanf(FOV * M_PI / 360);
    // GLfloat A = (FAR_Z + NEAR_Z) / (NEAR_Z - FAR_Z);
    // GLfloat B = 2 * FAR_Z * NEAR_Z / (NEAR_Z - FAR_Z);
    // glm::mat4 projectM = glm::mat4({
    //     {OneOverTanHalfFov / ASPECT_RATIO, 0.0, 0.0, 0.0},
    //     {0.0, OneOverTanHalfFov, 0.0, 0.0},
    //     {0.0, 0.0, -A, B},
    //     {0.0, 0.0, 1.0, 0.0},
    // });
    glm::mat4 projectM = glm::perspectiveFov(FOV * glm::pi<float>() / 180, WINDOW_WIDTH, WINDOW_HEIGHT, NEAR_Z, FAR_Z);
    // glm::vec3 N = glm::normalize(cameraFront);
    // glm::vec3 V = glm::normalize(cameraUp);
    // glm::vec3 U = glm::cross(N, V);
    // glm::mat4 viewM = glm::mat4({
    //     {U.x, U.y, U.z, glm::dot(U, cameraPosition)},
    //     {V.x, V.y, V.z, glm::dot(V, cameraPosition)},
    //     {-N.x, -N.y, -N.z, glm::dot(N, cameraPosition)},
    //     {0.0, 0.0, 0.0, 1.0},
    // });
    glm::mat4 viewM = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 finalMatrix = projectM * viewM * translateM * rotateM * scaleM;
    glUniformMatrix4fv(WVP, 1, GL_FALSE, &finalMatrix[0][0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, (sizeof(cube_index_data) / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

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
        // cameraAngle -= deltaCameraAngle;
        break;
    case 69:
    case 101:
        // cameraAngle += deltaCameraAngle;
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
    // glutInitContextVersion(3, 3);
    // glutInitContextProfile(GLUT_CORE_PROFILE);
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    textureData = stbi_load("container.jpg", &textureWidth, &textureHeight, &textureNrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    stbi_image_free(textureData);

    CreateBuffers();
    CompileShaders();
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glutMainLoop();

    return 0;
}