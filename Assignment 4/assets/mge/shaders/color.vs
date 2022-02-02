//DIFFUSE COLOR VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform vec3 directionalLightVector=vec3(1,-1,-1);

out float diffuseIntensity;

void main( void ){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);

	//make sure normal is in same space as light direction. You should be able to explain the 0.
    vec3 worldNormal = vec3 (modelMatrix * vec4 (normal,0));
	
	//take the dot of the direction from surface to light with the world surface normal
    diffuseIntensity = dot(normalize(directionalLightVector), normalize (worldNormal));
}
