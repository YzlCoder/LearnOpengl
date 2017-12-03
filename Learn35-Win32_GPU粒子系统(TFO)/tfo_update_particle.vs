attribute vec4 pos;
attribute vec4 mess;

out vec4 vs_Mess;

void main()
{
	vs_Mess = mess + vec4(0.0001, 0.0, 0.0, 0.0);
    gl_Position=pos+vec4(0.0,0.0001,0.0,0.0);
}