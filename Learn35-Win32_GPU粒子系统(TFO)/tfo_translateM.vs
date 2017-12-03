attribute vec4 pos;
attribute vec4 mess;

uniform mat4 M;

out vec4 o_mess;
void main()
{
    gl_Position=M*pos;
	o_mess = mess;
}