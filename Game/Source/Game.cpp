#include "Framework.h"

#include "Game.h"
#include "GameObjects/Cube.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "Meshes/Shapes.h"
#include "GameEvents/GameEvents.h"
#include "Scenes/PhysicsScene.h"

Game::Game(fw::FWCore& fwCore)
    : m_FWCore( fwCore )
{
    Init();
}

Game::~Game()
{
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

    for (auto& it : m_Materials)
    {
        delete it.second;
    }

    for( auto& it : m_SpriteSheets )
    {
        delete it.second;
    }

    for (auto& it : m_Scenes)
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
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glFrontFace(GL_CW);

    m_Meshes["Sprite"] = new fw::Mesh( GL_TRIANGLE_STRIP, g_SpriteVerts );
    m_Meshes["Cube"] = new fw::Mesh(GL_TRIANGLES, g_CubeVerts);

    m_Shaders["Basic"] = new fw::ShaderProgram( "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag" );

    m_Textures["Sprites"] = new fw::Texture( "Data/Textures/Sprites.png" );
    m_Textures["Cube"] = new fw::Texture("Data/Textures/CubeTexture.png");

    m_SpriteSheets["Sprites"] = new fw::SpriteSheet( "Data/Textures/Sprites.json", m_Textures["Sprites"] );

    m_Materials["Sokoban"] = new fw::Material(m_Shaders["Basic"], m_Textures["Sprites"], fw::Color4f::Red());
    m_Materials["Cube"] = new fw::Material(m_Shaders["Basic"], m_Textures["Cube"], fw::Color4f::Blue());


    //m_Scenes["Cube"] = new CubeScene();
    m_Scenes["Physics"] = new PhysicsScene(this);

    m_pCurrentScene = m_Scenes["Physics"];
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    m_pCurrentScene->StartFrame(deltaTime);
}

void Game::OnEvent(fw::Event* pEvent)
{
    m_pCurrentScene->OnEvent(pEvent);
}

void Game::Update(float deltaTime)
{
    ImGui::ShowDemoWindow();
 
    m_pCurrentScene->Update(deltaTime);
}

void Game::Draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_pCurrentScene->Draw();

    m_pImGuiManager->EndFrame();
}
