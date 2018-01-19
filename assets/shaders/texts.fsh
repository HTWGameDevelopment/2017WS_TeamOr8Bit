#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 5) uniform sampler2D diffuse;
layout(location = 9) uniform vec3 fg;

in vec2 out_uv;

out vec4 color;

void main() {
    color = vec4(fg.x,fg.y,fg.z,texture2D(diffuse, out_uv).x);
}
