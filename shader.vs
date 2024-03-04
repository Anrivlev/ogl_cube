#version 330

layout(location = 0) in vec3 in_Position;
uniform mat4 WVP;

layout(location = 1) in vec3 in_color;

out vec3 color;
out vec4 position;

void main() {
    position = WVP * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    gl_Position = position;
    color = in_color;
}