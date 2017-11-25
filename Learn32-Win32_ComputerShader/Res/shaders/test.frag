#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D texture2d;
varying vec2 V_texcoord;
void main()
{
	gl_FragColor = texture2D(texture2d, V_texcoord);
}