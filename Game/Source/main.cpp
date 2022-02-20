#include "Framework.h"

#include "Game.h"
#include "DefaultSettings.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    {
        fw::FWCore fwCore(c_windowSize.x, c_windowSize.y);

		fwCore.SetWindowTitle("Scene Changer");
		fwCore.SetIcon("../../Game/Data/TV.ico");

        Game game(fwCore);
		
        fwCore.Run(game);

        fwCore.Shutdown();
    }
}
