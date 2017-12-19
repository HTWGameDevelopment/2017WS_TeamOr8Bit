#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 3) uniform vec3 light_world;

layout(location = 5) uniform vec3 uni_color;

in vec3 camera_camera;
in vec3 light_camera;
in vec3 normal_camera;

out vec3 color;

void main() {
    vec3 n = normalize(normal_camera);
    vec3 l = normalize(light_camera);
    float cosTheta = clamp(dot(n,l),0,1);
    color = cosTheta * uni_color;
}
