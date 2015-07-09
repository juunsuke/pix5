in vec2 pos;
in vec4 col;
in vec2 texcoord;

varying vec4 x_col;
varying vec2 x_texcoord;


void main()
{
	gl_Position = gl_ProjectionMatrix * vec4(pos, 0, 1);
	x_col = col;
	x_texcoord = texcoord;
}

