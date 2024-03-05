#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 camera;

void main() {
    TexCoords = aPos;
    gl_Position = camera * vec4(aPos * 20.0 , 1.0);
}  