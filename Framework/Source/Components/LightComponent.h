#pragma once

#include "Component.h"
#include "Math/Vector.h"

namespace fw {

enum class LightType
{
    Directional,
    PointLight,
    SpotLight
};

struct LightFixture
{
    LightType type;
    Color4f ambient;
    Color4f diffuse;
    Color4f specular;
    float radius;
    float powerFactor;
};

class LightComponent : public Component
{
protected:
    LightFixture m_light;
    float m_spotCutoff = 0.f;

public:
	LightComponent(LightType type, Color4f color, float radius, float powerFactor);
    LightComponent(LightType type, Color4f color, float radius, float powerFactor, float spotCutoff);
    virtual ~LightComponent();

    static const char* GetStaticType() { return "LightingComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

    LightFixture* GetDetails() { return &m_light; }
    float GetCutoff() { return m_spotCutoff; }

    void SetDiffuse(Color4f diffuse);
    void SetPowerFactor(float powerFactor) { m_light.powerFactor = powerFactor; }
    void SetRadius(float radius) { m_light.radius = radius; }
    void SetCutoff(float spotCutoff) { m_spotCutoff = spotCutoff; }
};

} // namespace fw
