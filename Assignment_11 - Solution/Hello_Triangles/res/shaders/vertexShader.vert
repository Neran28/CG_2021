#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;

void main()
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1);
	color = vertexColor;
}