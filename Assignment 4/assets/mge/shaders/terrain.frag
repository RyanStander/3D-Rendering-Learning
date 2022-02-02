//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D splatMapTexture;
uniform sampler2D diffuseTexture1,diffuseTexture2,diffuseTexture3,diffuseTexture4;
uniform sampler2D heightMapTexture;
in vec2 texCoord;

out vec4 fragment_color;
out vec4 terrainHeighValue;

//Lighting stuff
uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 directionalLightColor;

uniform vec3 directionalLightForward;
uniform	mat4 modelMatrix;

float diffuseIntensity;
vec3 normalPoint;
in float heightYValue;

//uniform float heigthMapTexMov=0.001953125f;
float heigthMapTexMov=0.05f;

void main( void ) {
	//fragment_color = texture(splatMapTexture,texCoord);

	//Get texture colors based on coord
	vec4 splatTextureValues=texture(splatMapTexture,texCoord);
	vec4 diffuse1TextureValues=texture(diffuseTexture1,texCoord);
	vec4 diffuse2TextureValues=texture(diffuseTexture2,texCoord);
	vec4 diffuse3TextureValues=texture(diffuseTexture3,texCoord);
	vec4 diffuse4TextureValues=texture(diffuseTexture4,texCoord);
	
	//Get the colors values based on the splat map
	vec4 splatMap1Output= splatTextureValues.x*diffuse1TextureValues;
	vec4 splatMap2Output= splatTextureValues.y*diffuse2TextureValues;
	vec4 splatMap3Output= splatTextureValues.z*diffuse3TextureValues;
	vec4 splatMap4Output= splatTextureValues.w*diffuse4TextureValues;
	
	//Combine the values for fragment color output
	vec4 outputTexture = splatMap1Output+splatMap2Output+splatMap3Output+splatMap4Output;
	fragment_color=outputTexture;
	
	//LIGHTING STUFF
	//Get position samples based on uv with the offset of a pixel - rename to lefttopbot
	vec3 heightMapSample1=vec3(texture(heightMapTexture,vec2(texCoord.x+heigthMapTexMov,texCoord.y)));
	vec3 heightMapSample2=vec3(texture(heightMapTexture,vec2(texCoord.x-heigthMapTexMov,texCoord.y)));
	vec3 heightMapSample3=vec3(texture(heightMapTexture,vec2(texCoord.x,texCoord.y+heigthMapTexMov)));
	vec3 heightMapSample4=vec3(texture(heightMapTexture,vec2(texCoord.x,texCoord.y-heigthMapTexMov)));
	//Get 2 slope vectors
	heightMapSample1=vec3(texCoord.x+heigthMapTexMov,heightMapSample1.y,texCoord.y);
	heightMapSample2=vec3(texCoord.x-heigthMapTexMov,heightMapSample2.y,texCoord.y);
	heightMapSample3=vec3(texCoord.x,heightMapSample3.y,texCoord.y-heigthMapTexMov);
	heightMapSample4=vec3(texCoord.x,heightMapSample4.y,texCoord.y+heigthMapTexMov);
	vec3 directionalVector1=heightMapSample1-heightMapSample2;
	vec3 directionalVector2=heightMapSample4-heightMapSample3;
	//cross the 2 vectors to get their normal
	normalPoint=-cross(directionalVector1,directionalVector2);
	
	//make sure normal is in same space as light direction.
    vec3 worldNormal = vec3 (modelMatrix * vec4 (normalPoint,0));
	
	//Calculate the diffuse intensity
	diffuseIntensity = dot(normalize(directionalLightForward),normalize(worldNormal));
	
	//Calculate ambient term
	vec3 ambientTerm = ambientColor*ambientIntensity;

	//Calculate diffuse term
	vec3 preDiffused = vec3(outputTexture.x,outputTexture.y,outputTexture.z);
	vec3 diffuseTerm = (diffuseIntensity * directionalLightColor * preDiffused);
	
	//Calculate the color of each pixel
	vec3 final=diffuseTerm+ambientTerm;
	fragment_color = (vec4 (final,1));
	//fragment_color = (vec4 (worldNormal,1));
}
