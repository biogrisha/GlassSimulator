#version 330 core
out uvec3 FragColor;

flat in int trigId;

void main()
{
	FragColor = uvec3(trigId,1,1);
} 