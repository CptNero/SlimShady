#version 330 core

precision mediump float;

#define BLACK vec3(0.0, 0.0, 0.0)
#define GREEN vec3(0.2, 0.4, 0.3)

void main()
{
    vec2 uv = floor(gl_FragCoord.xy / 20.0);
    vec4 color;
    bool isEven = mod(uv.x + uv.y, 2.0) == 0.0;
    if(isEven) {
        color = vec4(BLACK, 1.0);
    }
    else {
        color = vec4(GREEN, 1.0);
    }
    gl_FragColor = color;
}





