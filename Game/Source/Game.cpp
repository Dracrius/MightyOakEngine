#include "Framework.h"

#include "Game.h"
#include "GameObjects/Cube.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "Meshes/Shapes.h"
#include "GameEvents/GameEvents.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/WaterScene.h"

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

    // Setup Meshes
    m_Meshes["Sprite"] = new fw::Mesh(GL_TRIANGLES, g_SpriteVerts, g_SpriteIndices);
    m_Meshes["Cube"] = new fw::Mesh(GL_TRIANGLES, g_CubeVerts);
    m_Meshes["Plane"] = new fw::Mesh();
    m_Meshes["Plane"]->CreatePlane(vec2(100.f, 100.f), ivec2(1000, 1000));

    // Setup Shaders
    m_Shaders["Basic"] = new fw::ShaderProgram( "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag" );
    m_Shaders["Water"] = new fw::ShaderProgram("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");

    // Setup Textures
    m_Textures["Sprites"] = new fw::Texture( "Data/Textures/Sprites.png" );
    m_Textures["Cube"] = new fw::Texture("Data/Textures/CubeTexture.png");
    m_Textures["Water"] = new fw::Texture("Data/Textures/WaterBW.png");

    // Setup Sprite Sheets
    m_SpriteSheets["Sprites"] = new fw::SpriteSheet( "Data/Textures/Sprites.json", m_Textures["Sprites"] );

    // Setup Materials
    m_Materials["Sokoban"] = new fw::Material(m_Shaders["Basic"], m_Textures["Sprites"], fw::Color4f::Red());
    m_Materials["Cube"] = new fw::Material(m_Shaders["Basic"], m_Textures["Cube"], fw::Color4f::Green());
    m_Materials["Water"] = new fw::Material(m_Shaders["Water"], m_Textures["Water"], fw::Color4f(15.f / 255, 103.f / 255, 227.f / 255, 1.f));

    // Setup Scenes
    m_Scenes["Physics"] = new PhysicsScene(this);
    m_Scenes["Cube"] = new CubeScene(this);
    m_Scenes["Water"] = new WaterScene(this);

    m_pCurrentScene = m_Scenes["Water"];
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    m_pCurrentScene->StartFrame(deltaTime);
}

void Game::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetEventType() == "SceneChangeEvent")
    {
        SceneChangeEvent* pSceneChange = static_cast<SceneChangeEvent*>(pEvent);
        m_pCurrentScene = m_Scenes[pSceneChange->GetSceneName()];
    }
    else
    {
        m_pCurrentScene->OnEvent(pEvent);
    }
}

void Game::Update(float deltaTime)
{
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Scenes");

    if (ImGui::Button("Physics"))
    {
        SceneChangeEvent* pSceneChange = new SceneChangeEvent("Physics");
        m_FWCore.GetEventManager()->AddEvent(pSceneChange);
    }

    if (ImGui::Button("Cube"))
    {
        SceneChangeEvent* pSceneChange = new SceneChangeEvent("Cube");
        m_FWCore.GetEventManager()->AddEvent(pSceneChange);
    }

    if (ImGui::Button("Water"))
    {
        SceneChangeEvent* pSceneChange = new SceneChangeEvent("Water");
        m_FWCore.GetEventManager()->AddEvent(pSceneChange);
    }

    ImGui::End();
 
    m_pCurrentScene->Update(deltaTime);
}

void Game::Draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_pCurrentScene->Draw();

    m_pImGuiManager->EndFrame();
}
