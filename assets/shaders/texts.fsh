#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 5) uniform sampler2D diffuse;

in vec2 out_uv;

out vec4 color;

void main() {
    color = vec4(1,1,1,texture2D(diffuse, out_uv).x);
}
