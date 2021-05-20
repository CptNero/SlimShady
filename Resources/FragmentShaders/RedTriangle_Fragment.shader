#version 410 core

uniform float u_RedTriangleTime;
uniform int u_RedTriangleWidth;
uniform int u_RedTriangleHeight;

uniform sampler2D u_RedTriangleTexture0;

in vec2 TexCoord;
in vec3 OurColor;

void main()
{
	gl_FragColor = texture(u_RedTriangleTexture0, TexCoord);
}