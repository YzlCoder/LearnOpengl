attribute vec4 pos;
attribute vec4 mess;

uniform mat4 V;

out vec4 vs_Color;
out vec4 vs_Mess;


void main()
{
    gl_Position = V*pos;
	vs_Mess = mess;
	vs_Color.a = pow(sin(3.14 * (mess.x / 5.0)), 2.0);
}