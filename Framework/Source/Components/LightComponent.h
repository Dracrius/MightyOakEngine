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
    Color4f ambient;
    Color4f diffuse;
    Color4f specular;
    float powerFactor;
};

class LightComponent : public Component
{
protected:
    LightType m_type = LightType::PointLight;
    LightFixture m_light;

public:
	LightComponent(LightType type, Color4f color, float powerFactor);
    virtual ~LightComponent();

    static const char* GetStaticType() { return "LightingComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

    LightType GetLightType() { return m_type; }
    LightFixture GetDetails() { return m_light; }

    void SetDiffuse(Color4f diffuse);
    void SetPowerFactor(float powerFactor) { m_light.powerFactor = powerFactor; }
};

} // namespace fw
