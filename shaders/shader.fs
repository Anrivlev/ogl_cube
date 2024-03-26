#version 330 core

in vec3 color;
in vec2 texture;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {
    // FragColor = vec4(color, 1.0);
    FragColor = (0.5 * vec4(color, 1.0) + vec4(0.5)) * texture2D(ourTexture, texture);
    // FragColor = texture2D(ourTexture, texture);
    // FragColor = vec4(texture.x, texture.y, texture.y, 1.0f);
}
