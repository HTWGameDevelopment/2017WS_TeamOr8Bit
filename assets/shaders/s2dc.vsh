#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec2 vertex;

layout(location = 0) uniform vec2 position;
layout(location = 1) uniform vec2 scale;

void main() {
    gl_Position = vec4(position + vertex * scale, 0, 1);
}
