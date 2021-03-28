
#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 OurColor;
out vec2 TexCoord;

uniform mat4 u_TanisaModel;
uniform mat4 u_TanisaView;
uniform mat4 u_TanisaProjection;

void main()
{
	gl_Position = u_TanisaProjection * u_TanisaView * u_TanisaModel * position;
	gl_Position += 1;
	OurColor = aColor;
	TexCoord = aTexCoord;
}





