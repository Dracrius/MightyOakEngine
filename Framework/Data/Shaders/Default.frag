
uniform sampler2D u_Texture;
uniform bool u_HasTexture;
uniform vec4 u_MaterialColor;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
	if(u_HasTexture)
	{
	    gl_FragColor = texture2D( u_Texture, v_UVCoord ) * v_Color;
	}
	else
	{
		gl_FragColor = u_MaterialColor;
	}
}
