#version 330

in vec3 color;
in vec2 texture;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {
    //FragColor = vec4(color, 1.0);
    FragColor = texture2D(ourTexture, texture);
}
