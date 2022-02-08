
uniform sampler2D u_Texture; // = 0;
uniform vec4 u_MaterialColor;
uniform float u_Time;

varying vec2 v_UVCoord;
varying vec4 v_Color;
varying float v_ObjectYPos;

void main()
{
    vec4 shade = vec4(v_ObjectYPos,v_ObjectYPos,v_ObjectYPos,1);

    vec2 uvP = v_UVCoord;
    uvP.x += sin(u_Time + v_UVCoord.y * 10.0) * 0.08;
    uvP.y -= sin(u_Time + v_UVCoord.x * 15.0) * 0.12;

    vec2 uvS = v_UVCoord;
    uvS.x += sin(u_Time - v_UVCoord.y * 10.0) * 0.08;
    uvS.y -= sin(u_Time - v_UVCoord.x * 15.0) * 0.12;

    vec4 primaryWater = texture2D( u_Texture, uvP ) + u_MaterialColor;

    vec4 secondaryWater = texture2D( u_Texture, uvS + vec2(u_Time / 21.0, 0.5)) * 0.1;
    secondaryWater.a = 0.0;

    gl_FragColor = primaryWater - secondaryWater;
    gl_FragColor.a = u_MaterialColor.a;

}
