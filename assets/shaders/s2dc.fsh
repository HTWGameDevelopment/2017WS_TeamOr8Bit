#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 8) uniform vec3 uni_color;

out vec4 color;

void main() {
    color = vec4(uni_color.xyz, 1);
}
