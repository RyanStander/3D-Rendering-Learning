//LIT COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in float diffuseIntensity;

uniform vec3 diffuseColor;
uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 directionalLightColor;

out vec4 fragment_color;

void main( void ) {
	//Create an ambient term so that we get a blend of diffuse coolor and ambient color
	//vec3 ambientTerm = (ambientIntensity*ambientColor)*diffuseColor;
	//fragment_color = vec4 (ambientTerm,1);
	
	
	vec3 ambientTerm = ambientColor*ambientIntensity;

	vec3 diffuseTerm = diffuseIntensity * directionalLightColor * diffuseColor;
	vec3 final=diffuseTerm+ambientTerm;
	fragment_color = vec4 (diffuseTerm,1);
	
	//Possible issue:
	//diffuse color - Correct
	//diffuseIntensity
	//directionalLightColor-Correct
}