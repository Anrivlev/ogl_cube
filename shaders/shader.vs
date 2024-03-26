#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texture;

uniform mat4 WVP;

out vec3 color;
out vec4 position;
out vec2 texture;

void main() {
    position = WVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);
    gl_Position = position;
    color = in_color;
    texture = in_texture;
}