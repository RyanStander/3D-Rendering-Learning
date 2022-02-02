//LIT COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 worldNormal;
in vec3 worldPosition;

uniform vec3 diffuseColor;
uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightVector;

//spec vals new
uniform float shininess;
uniform vec3 specularColor;
uniform vec3 cameraPosition;

out vec4 fragment_color;

void main( void ) {
	//Create an ambient term so that we get a blend of diffuse coolor and ambient color
	//vec3 ambientTerm = (ambientIntensity*ambientColor)*diffuseColor;
	//fragment_color = vec4 (ambientTerm,1);
	
	vec3 ambientTerm = ambientColor*ambientIntensity;

	//direction from surface to light
	vec3 lightDirection = lightVector-worldPosition;
	
	//take the dot of the direction from surface to light with the world surface normal
	//make sure the value is not lower than 0.
    float diffuseIntensity = max(dot(normalize(lightDirection), normalize (worldNormal)),0);
	//apply attenuations by getting the distance between the light position and the world position of the object fragment
	diffuseIntensity/=(.01f+distance(lightVector,worldPosition));

	//calculate specular Term
	//light ray reflected in the world normal
	vec3 r = reflect(lightDirection,normalize(worldNormal));
	//direction from viewer to viewed poin
	vec3 v = worldPosition-cameraPosition;	
	vec3 specularTerm=pow(max(dot(normalize(r),normalize(v)),0),shininess)*lightColor*specularColor;

	vec3 diffuseTerm = diffuseIntensity * lightColor * diffuseColor;
	vec3 final=specularTerm+ambientTerm+diffuseTerm;
	//fragment_color = vec4 (diffuseTerm,1);
	
	fragment_color = vec4 (final,1);
}