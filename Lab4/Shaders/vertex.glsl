#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

uniform mat4 E;
uniform mat4 P;

out vec3 interpolatedNormal;
out vec2 st;
out vec3 Normal;

uniform mat4 OT;


void main() {

vec3 transformedNormal = Normal;
interpolatedNormal = normalize(transformedNormal);

gl_Position = P * E * vec4(Position, 1.0); // Special, required output
//interpolatedNormal = Normal; // Will be interpolated across the triangle
st = TexCoord; // Will also be interpolated across the triangle
}
