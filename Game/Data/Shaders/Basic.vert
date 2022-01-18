#define PI 3.14159265358979323846

attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_UVCoord;

uniform vec2 u_ObjectTranslation;
uniform vec2 u_ObjectScale;
uniform vec2 u_CameraTranslation;
uniform vec2 u_ProjectionScale;
uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;
uniform float u_Time;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
    float objectAngle = 0.0f;
    float cameraAngle = -90.0/180.0 * PI; //1 * u_Time; //PI/4;

    vec2 objectSpacePosition = a_Position;

    vec2 worldSpacePosition = objectSpacePosition*u_ObjectScale; 
    float newX = worldSpacePosition.x * cos(objectAngle) - worldSpacePosition.y * sin(objectAngle);
    float newY = worldSpacePosition.x * sin(objectAngle) + worldSpacePosition.y * cos(objectAngle);
    worldSpacePosition = vec2(newX, newY) + u_ObjectTranslation;

    vec2 viewSpacePosition = worldSpacePosition + u_CameraTranslation;
    newX = viewSpacePosition.x * cos(cameraAngle) - viewSpacePosition.y * sin(cameraAngle);
    newY = viewSpacePosition.x * sin(cameraAngle) + viewSpacePosition.y * cos(cameraAngle);
    viewSpacePosition = vec2(newX, newY);

    vec2 clipSpacePosition = viewSpacePosition * u_ProjectionScale;
    gl_Position = vec4( clipSpacePosition, 0.0, 1.0 );
    
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
    v_Color = a_Color;
}
