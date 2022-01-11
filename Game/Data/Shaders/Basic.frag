
uniform sampler2D u_Texture; // = 0;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
    gl_FragColor = texture2D( u_Texture, v_UVCoord ) * v_Color;
}
