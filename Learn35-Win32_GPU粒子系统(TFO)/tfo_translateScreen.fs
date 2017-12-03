
uniform sampler2D U_MainTexture;
varying vec4 V_Color;
varying vec2 V_Texcoord;

void main()
{
    gl_FragColor=vec4(vec3(1.0,0.6,0.2),texture2D(U_MainTexture,V_Texcoord).a * V_Color.a);
}