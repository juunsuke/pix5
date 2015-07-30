
varying vec4 x_col;
varying vec2 x_texcoord;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	x_col = gl_Color;
	x_texcoord = gl_MultiTexCoord0;
}

