#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 OurColor;
out vec2 TexCoord;

uniform mat4 u_RedTriangleModel;
uniform mat4 u_RedTriangleView;
uniform mat4 u_RedTriangleProjection;

void main()
{
	gl_Position = u_RedTriangleProjection * u_RedTriangleView 
	* u_RedTriangleModel * position;
	OurColor = aColor;
	TexCoord = aTexCoord;
}


