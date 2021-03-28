
#version 410 core

uniform float u_TanisaTime;
uniform int u_TanisaWidth;
uniform int u_TanisaHeight;

uniform sampler2D u_TanisaTexture0;

in vec2 TexCoord;
in vec3 OurColor;

void main()
{
	gl_FragColor = texture(u_TanisaTexture0, TexCoord) * vec4(OurColor, 1.0);
}










