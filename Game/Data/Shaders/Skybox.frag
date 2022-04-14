
uniform samplerCube u_CubemapTexture;
uniform sampler2D u_Texture;
uniform vec4 u_MaterialColor;

varying vec2 v_UVCoord;
varying vec4 v_Color;

varying vec3 v_Normal;
varying vec3 v_ObjectSpaceSurfacePos;

void main()
{
    gl_FragColor = textureCube(u_CubemapTexture, v_ObjectSpaceSurfacePos);
}
