#version 330 core

layout (location = 0) in vec3 in_Position;
uniform mat4 transformMatrix;

void main() {
    gl_Position = transformMatrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}