#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 u_YikesModel;
uniform mat4 u_YikesView;
uniform mat4 u_YikesProjection;

void main()
{
	gl_Position = u_YikesProjection * u_YikesView * u_YikesModel * position;
	ourColor = aColor;
	TexCoord = aTexCoord;
}




