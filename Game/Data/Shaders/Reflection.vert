attribute vec3 a_Position;
attribute vec3 a_Normal;

uniform mat4 u_WorldMatrix;
uniform mat4 u_WVPMatrix;
uniform mat4 u_NormalMatrix;

uniform vec3 u_CamPos;

varying vec3 v_ReflectedDir;

#define PI 3.14159265358979323846

void main()
{
    vec4 objectSpacePosition = vec4(a_Position, 1);
    vec4 worldSpacePosition = u_WorldMatrix * objectSpacePosition;
    //vec4 viewSpacePosition = u_ViewMatrix * worldSpacePosition;
    //vec4 clipSpacePosition = u_ProjecMatrix * viewSpacePosition;

    vec4 clipSpacePosition = u_WVPMatrix * objectSpacePosition;

    gl_Position = clipSpacePosition;

    vec4 normal = u_NormalMatrix * vec4(a_Normal, 0 );

    vec3 dirToSurface = worldSpacePosition.xyz - u_CamPos;
    v_ReflectedDir = reflect(dirToSurface, normal.xyz);
}
