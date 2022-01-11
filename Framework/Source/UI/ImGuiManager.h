#pragma once

struct ImDrawData;

namespace fw {

class FWCore;

class ImGuiManager
{
public:
    ImGuiManager(FWCore* pFramework);
    virtual ~ImGuiManager();

    void Init();
    void Shutdown();

    void OnFocusLost();

    void StartFrame(float deltaTime);
    void EndFrame();

protected:
    void ClearInput();

    void RenderDrawLists(ImDrawData* draw_data);

    bool CreateFontsTexture();
    bool CreateDeviceObjects();
    void InvalidateDeviceObjects();

protected:
    FWCore* m_pFramework = nullptr;

    GLuint m_FontTexture = 0;
    int m_ShaderHandle = 0;
    int m_VertHandle = 0;
    int m_FragHandle = 0;
    int m_AttribLocationTex = 0;
    int m_AttribLocationProjMtx = 0;
    int m_AttribLocationPosition = 0;
    int m_AttribLocationUV = 0;
    int m_AttribLocationColor = 0;
    unsigned int m_VBOHandle = 0;
    unsigned int m_VAOHandle = 0;
    unsigned int m_ElementsHandle = 0;
};

} // namespace fw
