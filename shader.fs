#version 330

in vec4 position;

out vec4 FragColor;

void main() {
    FragColor = vec4(position.x, position.y, position.z, 1.0);
}
