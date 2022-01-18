attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform mat4 u_WorldMatrix;
uniform vec2 u_CameraTranslation;
uniform vec2 u_ProjectionScale;
uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;
uniform float u_Time;

varying vec2 v_UVCoord;
varying vec4 v_Color;

#define PI 3.14159265358979323846

void main()
{
    float cameraAngle = -0.0/180.0 * PI;

    vec4 objectSpacePosition = vec4(a_Position, 0, 1);
    vec4 worldSpacePosition = u_WorldMatrix * objectSpacePosition;

    vec2 viewSpacePosition = worldSpacePosition.xy + u_CameraTranslation;
    float newX = viewSpacePosition.x * cos(cameraAngle) - viewSpacePosition.y * sin(cameraAngle);
    float newY = viewSpacePosition.x * sin(cameraAngle) + viewSpacePosition.y * cos(cameraAngle);
    viewSpacePosition = vec2(newX, newY);

    vec2 clipSpacePosition = viewSpacePosition * u_ProjectionScale;
    gl_Position = vec4( clipSpacePosition, 0.0, 1.0 );
    
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
    v_Color = a_Color;
}
