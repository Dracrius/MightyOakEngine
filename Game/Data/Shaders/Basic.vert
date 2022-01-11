
attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform vec2 u_ObjectTranslation;
uniform vec2 u_ObjectScale;
uniform vec2 u_CameraTranslation;
uniform vec2 u_ProjectionScale;
uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
    vec2 objectSpacePosition = a_Position;
    vec2 worldSpacePosition = objectSpacePosition*u_ObjectScale + u_ObjectTranslation;
    vec2 viewSpacePosition = worldSpacePosition + u_CameraTranslation;
    vec2 clipSpacePosition = viewSpacePosition * u_ProjectionScale;
    gl_Position = vec4( clipSpacePosition, 0.0, 1.0 );
    
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
    v_Color = a_Color;
}
