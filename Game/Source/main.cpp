#include "Framework.h"

#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    fw::FWCore fwCore( 600, 600 );
    
    Game game( fwCore );

    fwCore.Run( game );

    fwCore.Shutdown();
}
