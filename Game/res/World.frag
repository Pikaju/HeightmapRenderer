#version 330

#include "Fog.glh"

uniform vec3 cameraPosition;

in vec3 fragmentPosition;
in vec3 fragmentNormal;

out vec4 pixelColor;

void main()
{
	float light = dot(-fragmentNormal, normalize(vec3(0.25, -1.0, 0.5))) * 0.5 + 0.5;
	vec3 color = mix(vec3(0.3, 0.3, 0.3), mix(vec3(0.0, 0.6, 0.0), vec3(1.0), clamp((fragmentPosition.y - 16.0) / 4.0, 0.0, 1.0)), pow(fragmentNormal.y, 5));
	pixelColor = vec4(color * light * (1.0 - getFog(fragmentPosition, cameraPosition)) * vec3(0.5, 0.5, 0.7), 1.0);
}