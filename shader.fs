#version 330

in vec3 in_Position;

out vec4 fragColor;

void main()
{
    fragColor = vec4(in_Position.x,in_Position.y, in_Position.z, 1.0);
}
