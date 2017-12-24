#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 uv;

layout(location = 0) uniform vec2 position;
layout(location = 1) uniform vec2 scale;

out vec2 out_uv;

void main() {
    gl_Position = vec4(position + vertex * scale, 0, 1);
    out_uv = uv;
}
