#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;

void main()
{
	fragmentPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
	fragmentNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}