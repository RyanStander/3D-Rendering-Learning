#version 330

in vec3 fColor;
in vec2 fuvs;
out vec4 sColor;

uniform vec2 amountOfSquares;

void main (void) {
    //Original
	//sColor = vec4(fColor,1);
	
	//Testing for checker size, the magic number value determines the amount of checkers
	//sColor= vec4(mod(floor(2*fuvs.x)+floor(2*fuvs.y), 2),0,1,1);
	
	//Creating a checker with the use of a uniform
	//Takes value which is either closest to 0/1 based on uv
    //multiplies chosen table size with uv coordinates, result is rounded down to closes ingteger. 
    //mod with 2 to either get 0 or 1.
	//currently blue and white
	sColor= vec4(mod(floor(amountOfSquares.x*fuvs.x)+floor(amountOfSquares.y*fuvs.y), 2),1,1,1);
}

