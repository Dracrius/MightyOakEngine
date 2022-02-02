#pragma once

namespace fw {

class EventManager;
class GameCore;
class MyGLContext;

class FWCore
{
public:
    FWCore(int width, int height);
    virtual ~FWCore();

    bool Init(int width, int height);
    int Run(GameCore& game);
    void Shutdown();

    void SetWindowSize(int width, int height);

    bool IsKeyDown(int value);
    bool IsMouseButtonDown(int id);
    void GetMouseCoordinates(int* mx, int* my);

    unsigned int GetWindowWidth() { return m_WindowWidth; }
    unsigned int GetWindowHeight() { return m_WindowHeight; }

    void SetEscapeKeyWillQuit(bool value) { m_EscapeKeyWillQuit = value; }

    void SwapBuffers();

    EventManager* GetEventManager() { return m_pEventManager; }

protected:
    void ResizeWindow(int width, int height);
    bool CreateGLWindow(char* title, int width, int height, unsigned char colorBits, unsigned char alphaBits, unsigned char zBits, unsigned char stencilBits, unsigned char multisampleSize, bool fullscreenflag);
    bool FailAndCleanup(const char* pMessage);
    void KillGLWindow(bool destroyInstance);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    bool m_EscapeKeyWillQuit = true;

    int m_WindowWidth = -1;
    int m_WindowHeight = -1;

    HWND m_hWnd = nullptr;
    HGLRC m_hRenderingContext = nullptr;
    HDC m_hDeviceContext = nullptr;
    HINSTANCE m_hInstance = nullptr;
    MyGLContext* m_pMyGLContext = nullptr;

    EventManager* m_pEventManager = nullptr;

    bool m_KeyStates[256] = {};
    bool m_MouseButtonStates[3] = {};
    bool m_OldKeyStates[256] = {};
    bool m_OldMouseButtonStates[3] = {};
    bool m_WindowIsActive = false;
    bool m_FullscreenMode = false;
};

} // namespace fw
