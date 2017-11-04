#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 3) uniform vec3 light_world;

layout(location = 4) uniform sampler2D diffuse;

in vec3 vertex_world;
in vec3 camera_camera;
in vec3 light_camera;
in vec3 normal_camera;
in vec2 uv_fragment;

out vec3 color;

void main() {
    vec3 n = normalize(normal_camera);
    vec3 l = normalize(light_camera);
    float distance = length(light_world - vertex_world);
    float cosTheta = clamp(dot(n,l),0,1);
    vec3 tex = texture(diffuse, uv_fragment).rgb;
    color = cosTheta * tex;
}
