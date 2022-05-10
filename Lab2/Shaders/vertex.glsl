#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

out vec3 interpolatedColor;

uniform float time;

uniform mat4 E;


void main() {
	//gl_Position = vec4(Position, 1.0);
	interpolatedColor = Color; // Pass interpolated color to fragment shader

	// Transform (x,y,z) vertex coordinates with the 4x4 matrix T
	gl_Position =  E * vec4(Position, 1.0);

}
