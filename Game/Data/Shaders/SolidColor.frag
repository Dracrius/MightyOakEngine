
uniform sampler2D u_Texture; // = 0;
uniform vec4 u_MaterialColor;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
    gl_FragColor = u_MaterialColor;
}
