#version 330 core
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 3) uniform vec3 light_world;

layout(location = 5) uniform vec3 uni_color;

layout(location = 8) uniform uvec2 uni_sel;

in vec3 pos_world;
in vec3 camera_camera;
in vec3 light_camera;
in vec3 normal_camera;

out vec3 color;

#assign MARKERCOUNT
#assign BOARDSIZE_X
#assign BOARDSIZE_Y

const uint BX = uint(BOARDSIZE_X);
const uint BY = uint(BOARDSIZE_Y);
const uint MC = uint(MARKERCOUNT);

layout(std140) uniform MarkerBlock {
    uvec4 array[BX * BY];
};

uint ismarked(uvec2 p, uint marker) {
    uint index = uint(p.x * BY + p.y);
    if(marker == uint(0)) return array[index].x;
    else if(marker == uint(1)) return array[index].y;
    else if(marker == uint(2)) return array[index].z;
    return array[index].w;
}

void main() {
    vec3 n = normalize(normal_camera);
    vec3 l = normalize(light_camera);
    float cosTheta = clamp(dot(n,l),0,1);

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
        if(any(lessThan(distances.yzw - distances.xxx, vec3(0.01, 0.01, 0.01)))) {
            cosTheta = 0;
        }
    } else if(all(lessThanEqual(distances.yyyy, distances))) {
        tcoord = vec2(xval.y, yval.y);
        if(any(lessThan(distances.xzw - distances.yyy, vec3(0.01, 0.01, 0.01)))) {
            cosTheta = 0;
        }
    } else if(all(lessThanEqual(distances.zzzz, distances))) {
        tcoord = vec2(xval.z, yval.z);
        if(any(lessThan(distances.xyw - distances.zzz, vec3(0.01, 0.01, 0.01)))) {
            cosTheta = 0;
        }
    } else if(all(lessThanEqual(distances.wwww, distances))) {
        tcoord = vec2(xval.w, yval.w);
        if(any(lessThan(distances.xyz - distances.www, vec3(0.01, 0.01, 0.01)))) {
            cosTheta = 0;
        }
    }

    color = 0.1 * uni_color + cosTheta * uni_color;

    if(tcoord.x >= 0 && tcoord.y >= 0 && tcoord.x < BX && tcoord.y < BY) {
        if(uvec2(tcoord) == uni_sel) {
            visvalue = 0.3;
        }
        if(ismarked(uvec2(tcoord), uint(2)) == uint(1)) {
            visvalue += 0.2;
        }
        if(ismarked(uvec2(tcoord), uint(1)) == uint(1)) {
            if(floor(mod(dot(pos_world.xz, glm::vec2(1, -1)) / 0.4, 2)) == 1)
                color = vec3(1, 0.2, 0.2) * (0.1 + cosTheta);
        }
        if(ismarked(uvec2(tcoord), uint(3)) == uint(1)) {
            if(floor(mod(dot(pos_world.xz, glm::vec2(-1, 1)) / 0.4, 2)) == 1)
                color = vec3(0, 0.551, 0.8) * (0.1 + cosTheta);
        } else if(ismarked(uvec2(tcoord), uint(3)) == uint(2)) {
            return;
        }
        color += visvalue * uni_color;
    }

}
