#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec3 vertex_model;

layout(location = 0) uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertex_model, 1);
}
