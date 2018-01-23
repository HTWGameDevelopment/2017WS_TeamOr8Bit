#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 3) uniform vec3 light_world;

in vec3 pos_world;

out uvec2 tileno;

#assign BOARDSIZE_X
#assign BOARDSIZE_Y

void main() {
    float visvalue = 0.1;

    float yval1 = floor(pos_world.z / 1.5);
    float xval1 = floor((pos_world.x + mod(yval1, 2) * 0.5 * 2 * 0.866) / (2 * 0.866));

    vec4 xval = vec4(0, 1, 0, 1) + xval1;
    vec4 yval = vec4(0, 0, 1, 1) + yval1;

    vec4 xpoints = vec4(2 * 0.866 * xval.x - mod(yval.x, 2) * 0.5 * 2 * 0.866,
                        2 * 0.866 * xval.y - mod(yval.y, 2) * 0.5 * 2 * 0.866,
                        2 * 0.866 * xval.z - mod(yval.z, 2) * 0.5 * 2 * 0.866,
                        2 * 0.866 * xval.w - mod(yval.w, 2) * 0.5 * 2 * 0.866);

    vec4 ypoints = yval * 1.5;

    vec4 distances = vec4(distance(pos_world.xz, vec2(xpoints.x, ypoints.x)),
                            distance(pos_world.xz, vec2(xpoints.y, ypoints.y)),
                            distance(pos_world.xz, vec2(xpoints.z, ypoints.z)),
                            distance(pos_world.xz, vec2(xpoints.w, ypoints.w)));

    vec2 tcoord;
    if(all(lessThanEqual(distances.xxxx, distances))) {
        tcoord = vec2(xval.x, yval.x);
    } else if(all(lessThanEqual(distances.yyyy, distances))) {
        tcoord = vec2(xval.y, yval.y);
    } else if(all(lessThanEqual(distances.zzzz, distances))) {
        tcoord = vec2(xval.z, yval.z);
    } else if(all(lessThanEqual(distances.wwww, distances))) {
        tcoord = vec2(xval.w, yval.w);
    }

    tileno = uvec2(tcoord);

}
