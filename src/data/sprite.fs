
varying vec4 x_col;
varying vec2 x_texcoord;

uniform sampler2D tex;

void main()
{
	gl_FragColor = x_col * texture2D(tex, x_texcoord);
}

