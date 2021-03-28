#version 410 core

#ifdef GL_ES
precision highp float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float u_YikesTime;
uniform int u_YikesWidth;
uniform int u_YikesHeight;

uniform sampler2D u_YikesTexture0;

in vec2 TexCoord;

const int ITERATIONS = 200;

vec3 square(vec3 z) {
    return vec3(
                z.x*z.x + z.y*z.y - z.z*z.z,
                2.0 * z.x * z.y,
                2.0 * z.x * z.z
               );
}

void main() {
    vec2 pos = (gl_FragCoord.xy/u_YikesWidth)-vec2(0.750,0.25);
    pos *= 4.0;
    
    vec3 c = vec3(pos.x, (sin(1.0)+1.0)* 1, pos.y);
    vec3 z = c;
    int escapetime = 0;
    
    for (int i = 0; i < ITERATIONS; i++) {
        z = square(z) + c;
        if (abs(z.x) > 10.) {
            escapetime = i;
            break;
        }
    }

    vec4 color = vec4(1.0-exp(-float(escapetime) / 5.0));
    
    //if (length(pos-vec2(0.25,0)) < c.y || length(pos-vec2(-2,0)) < c.y) color += vec3(1,-1,-1);
    
    gl_FragColor = texture(u_YikesTexture0, TexCoord) * color;
}


















