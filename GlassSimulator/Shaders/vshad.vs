#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec4 aNorm;


out vec4 pos;
flat out vec4 col;
out vec4 norm;
out vec4 fragPos;

uniform mat4 view;
uniform mat4 projection;



void main()
{
	norm =  normalize(aNorm);
	col = aCol;
	gl_Position = projection * view * vec4(aPos.xyz, 1.0f);
	pos = gl_Position;
	fragPos = aPos;
}