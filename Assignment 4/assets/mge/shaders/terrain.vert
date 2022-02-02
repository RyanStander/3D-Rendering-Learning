//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 mvpMatrix;
uniform sampler2D heightMapTexture;
uniform float heightScale;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord;
out float heightYValue;

void main( void ){
	
	texCoord = uv;
	
	//Sample the height map based on uv (texCoord)
	vec4 heightMapSample = texture(heightMapTexture,texCoord);
	//Get the height value with a scale value
	heightYValue=(heightMapSample.y)*heightScale;
	//Set the position values of the terrain
    gl_Position = mvpMatrix * vec4(vertex.x,heightYValue,vertex.z, 1.f);
	

}
