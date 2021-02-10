#version 410 core

#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float u_Time;
uniform int u_Width;
uniform int u_Height;

#define WAVELENGTH 0.01
#define WAVESPEED 20.0

float wave_height(vec2 p, vec2 c)
{
    float d = distance(p, c);
    return (1.0 - sin((d - WAVESPEED * u_Time))/ WAVELENGTH) / 2.0;
}

// main
void main(void)
{
    // vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 uv = gl_FragCoord.xy; // / resolution.xy;

    gl_FragColor = vec4(wave_height(uv, vec2(0.0, 0.0)),
			wave_height(uv, vec2(0.0, u_Height / 2.0)),
			wave_height(uv, vec2(0.0, u_Height)), 1.0);
}

