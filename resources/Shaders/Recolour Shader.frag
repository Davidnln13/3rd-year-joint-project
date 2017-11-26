#version 330
uniform sampler2D currentTexture;
vec4 newColor;
uniform float newR = 1;
uniform float newG = 1;
uniform float newB = 1;
uniform float newA = 1;

void main()
{
	//we set our new Vec4 values, using the uniform variables (set by user)
	newColor.r = newR;
	newColor.g = newG;
	newColor.b = newB;
	newColor.a = newA;

	//get the coordinate of the texel
	vec2 coord = gl_TexCoord[0].xy;
	vec4 pixel_color = texture2D(currentTexture, coord);
    gl_FragColor = pixel_color * newColor; //our new texel colour
}
