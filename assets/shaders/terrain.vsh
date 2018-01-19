#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec3 vertex_model;
layout(location = 2) in vec3 normal_model;

layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform mat4 m;
layout(location = 2) uniform mat4 v;

layout(location = 3) uniform vec3 light_world;

out vec3 pos_world;
out vec3 camera_camera;
out vec3 light_camera;
out vec3 normal_camera;

void main() {
    gl_Position = mvp * vec4(vertex_model, 1);
    pos_world = (m * vec4(vertex_model, 1)).xyz;
    camera_camera = vec3(0, 0, 0) - (v * m * vec4(vertex_model, 1)).xyz;
    light_camera = (v * vec4(-light_world, 0)).xyz;
    normal_camera = (v * m * vec4(normal_model, 0)).xyz;
}
