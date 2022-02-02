//DIFFUSE COLOR VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 projectionMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

out vec3 worldNormal;
out vec3 worldPosition;

void main( void ){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);

	//make sure normal is in same space as light direction.
	//Use a 0 for the w of the normal to prevent translation
     worldNormal = vec3 (modelMatrix * vec4 (normal,0));
	
	//make sure vertex is in same space as light position
	//use a 1 for the w of the vertex since translations are important
	 worldPosition = vec3(modelMatrix*vec4(vertex,1));
}
