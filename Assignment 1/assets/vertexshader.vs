#version 330

in vec3 vertex;
in vec3 color;
in vec2 uvs;
uniform vec2 offset;

out vec3 fColor;
out vec2 fuvs;

void main (void) {
    gl_Position = vec4(vertex,1) + vec4(offset,0,0);
    fColor = color;
	fuvs=uvs;
}

