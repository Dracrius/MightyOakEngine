attribute vec3 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform mat4 u_WVPMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;
uniform float u_Time;

varying vec2 v_UVCoord;
varying vec4 v_Color;

#define PI 3.14159265358979323846

void main()
{
    gl_Position = u_WVPMatrix * vec4(a_Position, 1);
    
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
    v_Color = a_Color;
}
