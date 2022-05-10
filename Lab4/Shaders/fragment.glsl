#version 330 core

out vec4 finalcolor;
//in vec3 interpolatedColor;

uniform float time;

uniform mat4 E;
uniform mat4 R;

in vec3 interpolatedNormal;
in vec2 st;
in vec3 Normal;



void main() {

vec3 lightDirection = vec3(0.4, 0.4, 0);
// vec3 L is the light direction
vec3 L = mat3(R) * lightDirection;
L = normalize(L);

// vec3 V is the view direction - (0,0,1) in view space
vec3 V = vec3(0.0f , 0.0f, 1.0f);

// vec3 N is the normal
vec3 N = interpolatedNormal;

// vec3 R is the computed reflection direction5
//vec3 R = normalizer(2.0 * dot(N,L)*N - L);

vec3 colorRGB = vec3(0.5f, 0.0f, 0.0f);
vec3 colorGreyscale = vec3(1.0, 1.0f, 1.0f);

// float n is the shininess''parameter
float n = 25.0f;

// vec3 ka is the ambient reflection color
vec3 ka = 1.0f * colorRGB;

// vec3 Ia is the ambient illumination color
vec3 Ia = 0.3f * colorGreyscale;

// vec3 kd is the diffuse surface reflection color
vec3 kd = 1.0f * colorRGB;

// vec3 Id is the diffuse illumination color
vec3 Id = 1.0f * colorGreyscale;

// vec3 ks is the specular surface reflection color
vec3 ks = 0.7f * colorGreyscale;

// vec3 Is is the specular illumination color
vec3 Is = 1.0f * colorGreyscale;

//float shading = dot(interpolatedNormal, lightDirection);
//shading = max(0.0, shading);  // Clamp negative values to 0.0

// This assumes that N, L and V are normalized.


vec3 Rv = 2.0 * dot(N, L) * N - L;   // Could also have used the function reflect()
float dotNL = max(dot(N, L), 0.0);  // If negative, set to zero
float dotRV = max(dot(Rv, V), 0.0);

if (dotNL == 0.0) {

dotRV = 0.0;  // Do not show highlight on the dark side
}
vec3 shadedcolor = Ia * ka + Id * kd * dotNL + Is * ks * pow(dotRV, n);

finalcolor = vec4(shadedcolor, 1.0);

//finalcolor = vec4(vec3(shading), 1.0);
}
