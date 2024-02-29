#version 330

in vec3 in_Position;
in mat4 transformMatrix;

void main() {
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}