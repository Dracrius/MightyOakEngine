#include "Framework.h"

#include "Game.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "Meshes/Shapes.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/Layouts.h"
#include "GameEvents/GameEvents.h"

Game::Game(fw::FWCore& fwCore)
    : m_FWCore( fwCore )
{
    Init();
}

Game::~Game()
{
    for( fw::GameObject* pObject : m_Objects )
    {
        delete pObject;
    }

    delete m_pPlayerController;

    delete m_pTilemap;

    delete m_pPhysicsWorld;

    for( auto& it : m_Meshes )
    {
        delete it.second;
    }

    for( auto& it : m_Shaders )
    {
        delete it.second;
    }

    for( auto& it : m_Textures )
    {
        delete it.second;
    }

    for( auto& it : m_SpriteSheets )
    {
        delete it.second;
    }

    delete m_pImGuiManager;
}

void Game::Init()
{
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore );

    // OpenGL settings.
    glPointSize( 10 );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    m_Meshes["Sprite"] = new fw::Mesh( GL_TRIANGLE_STRIP, g_SpriteVerts );
    m_Shaders["Basic"] = new fw::ShaderProgram( "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag" );
    m_Textures["Sprites"] = new fw::Texture( "Data/Textures/Sprites.png" );
    m_SpriteSheets["Sprites"] = new fw::SpriteSheet( "Data/Textures/Sprites.json", m_Textures["Sprites"] );

    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    m_pTilemap = new Tilemap( this, g_MainMap, ivec2(g_MainMapWidth, g_MainMapHeight), vec2(1.5f, 1.5f) );

    m_pCamera = new fw::Camera( this, vec2(1.5f*10,1.5f*10)/2, vec2(1/10.0f, 1/10.0f) );

    m_pPlayerController = new PlayerController();

    Player* pPlayer = new Player( this, m_Meshes["Sprite"], m_Shaders["Basic"], m_Textures["Sprites"], vec2(7.0f, 9.0f), m_pPlayerController );
    pPlayer->SetSpriteSheet( m_SpriteSheets["Sprites"] );
    pPlayer->SetTilemap( m_pTilemap );
    pPlayer->CreateBody(m_pPhysicsWorld, true, vec2(1.f, 1.f), 1.f);
    m_Objects.push_back( pPlayer );

    Player* pPlatform = new Player(this, m_Meshes["Sprite"], m_Shaders["Basic"], m_Textures["Sprites"], vec2(7.0f, 4.0f), m_pPlayerController);
    pPlatform->SetSpriteSheet(m_SpriteSheets["Sprites"]);
    pPlatform->SetTilemap(m_pTilemap);
    pPlatform->CreateBody(m_pPhysicsWorld, false, vec2(1.f, 1.f), 1.f);
    m_Objects.push_back(pPlatform);
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );
    m_pPlayerController->StartFrame();
}

void Game::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent( pEvent );

    if( pEvent->GetEventType() == RemoveFromGameEvent::GetStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }
}

void Game::Update(float deltaTime)
{
    ImGui::ShowDemoWindow();
     
    m_pPhysicsWorld->Update(deltaTime);

    for( auto it = m_Objects.begin(); it != m_Objects.end(); it++ )
    {
        fw::GameObject* pObject = *it;
        pObject->Update( deltaTime );
    }
}

void Game::Draw()
{
    glClearColor( 0.0f, 0.0f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    m_pTilemap->Draw( m_pCamera );

    for( auto it = m_Objects.begin(); it != m_Objects.end(); it++ )
    {
        fw::GameObject* pObject = *it;
        pObject->Draw( m_pCamera );
    }

    m_pImGuiManager->EndFrame();
}
