#version 330

layout(location = 0) in vec3 in_Position;
// uniform mat4 transformMatrix;

out vec4 position;

void main() {
    position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    gl_Position = position;
}