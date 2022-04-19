#pragma once
#include "Framework.h"
#include "DataTypes.h"

//Default Settings
const ivec2 c_windowSize = ivec2(1124, 644);
const ivec2 c_glRenderSize = ivec2(1024, 544);
//Aspect Ratio of OpenGl Window - Can be calculated here, using the OpenGL Render Size https://toolstud.io/photo/aspect.php?
const float c_aspectRatio = 1.88f; 

const std::string c_defaultScene = "Physics3D";
//List of Scenes: ["Physics"], ["Cube"], ["Water"], ["Obj"], ["ThirdPerson"], ["Assignment1"], ["Physics3D"]

const float c_animationLength = 0.12f;

const float c_shaunAnimationLength = 0.1f;
const float c_shaunIdleLength = 0.3f;

const float c_meteorSpawnDelay = 0.5f;
const float c_debrisLifeSpan = 3.f;

const vec2 c_gravity = vec2(0.f, -9.8f);

const vec3 c_centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
const vec3 c_cameraOffset = vec3(0.f, 0.f, -20.f);

//Colors
const Color4f c_defaultBackground = Color4f(110.f / 255, 158.f / 255, 251.f / 255, 1.f);
const Color4f c_defaultObjColor = Color4f::Grey();
const Color4f c_defaultWaterColor = Color4f(15.f / 255, 103.f / 255, 227.f / 255, 1.f);

//Player
const vec2 c_playerCollider = vec2(0.83f, 0.83f);
const vec2 c_shaunCollider = vec2(0.97f, 0.64f);
const float c_playerSpeed = 4.f;
const float c_shaunSpeed = 8.f;
const float c_jumpTimer = 0.5f;
