attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjecMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;
uniform float u_Time;

varying vec2 v_UVCoord;
varying vec4 v_Color;

#define PI 3.14159265358979323846

void main()
{
    vec4 objectSpacePosition = vec4(a_Position, 0, 1);
    vec4 worldSpacePosition = u_WorldMatrix * objectSpacePosition;
    vec4 viewSpacePosition = u_ViewMatrix * worldSpacePosition;
    vec4 clipSpacePosition = u_ProjecMatrix * viewSpacePosition;

    gl_Position = clipSpacePosition;
    
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
    v_Color = a_Color;
}
