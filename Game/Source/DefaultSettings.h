#pragma once
#include "Framework.h"
#include "DataTypes.h"

//Default Settings
const ivec2 c_windowSize = ivec2(1200, 700);
const ivec2 c_glRenderSize = ivec2(600, 600);

const std::string c_defaultScene = "Physics";
//List of Scenes: ["Physics"], ["Cube"], ["Water"], ["Obj"]

const float c_animationLength = 0.12f;
const vec2 c_gravity = vec2(0.f, -9.8f);

const vec3 c_centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
const vec3 c_cameraOffset = vec3(0.f, 0.f, -20.f);

//Colors
const Color4f c_defaultBackground = Color4f(0.0f, 0.0f, 0.2f, 1.0f);
const Color4f c_defaultObjColor = Color4f::Grey();
const Color4f c_defaultWaterColor = Color4f(15.f / 255, 103.f / 255, 227.f / 255, 1.f);

//Player
const vec2 c_playerCollider = vec2(0.83f, 0.83f);
const float c_playerSpeed = 250.f;
const float c_jumpTimer = 0.5f;

//Astroids
const int c_astroidPoolSize = 10;
const float c_astroidRadius = 0.4f;
const float c_astroidSpeed = 1.f;