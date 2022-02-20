#include "Framework.h"

#include "Game.h"
#include "GameObjects/Player.h"
#include "GameObjects/PlayerController.h"
#include "Meshes/Shapes.h"
#include "GameEvents/GameEvents.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/WaterScene.h"
#include "Scenes/ObjScene.h"
#include "DefaultSettings.h"

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
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
    m_Meshes["Obj"] = new fw::Mesh();


    // Setup Shaders
    m_Shaders["Basic"] = new fw::ShaderProgram( "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag" );
    m_Shaders["Water"] = new fw::ShaderProgram("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");
    m_Shaders["SolidColor"] = new fw::ShaderProgram("Data/Shaders/SolidColor.vert", "Data/Shaders/SolidColor.frag");

    // Setup Textures
    m_Textures["Sprites"] = new fw::Texture( "Data/Textures/Sprites.png" );
    m_Textures["Cube"] = new fw::Texture("Data/Textures/CubeTexture.png");
    m_Textures["Water"] = new fw::Texture("Data/Textures/WaterTile.png");
    m_Textures["Arcade_Cabinet"] = new fw::Texture("Data/Textures/Arcade_Cabinet.png");
    m_Textures["Arcade_Floor"] = new fw::Texture("Data/Textures/Arcade_Cabinet_Floor_Low_Light.png");

    // Setup Sprite Sheets
    m_SpriteSheets["Sprites"] = new fw::SpriteSheet( "Data/Textures/Sprites.json", m_Textures["Sprites"] );

    // Setup Materials
    m_Materials["Sokoban"] = new fw::Material(m_Shaders["Basic"], m_Textures["Sprites"], fw::Color4f::Red());
    m_Materials["Cube"] = new fw::Material(m_Shaders["Basic"], m_Textures["Cube"], fw::Color4f::Green());
    m_Materials["Water"] = new fw::Material(m_Shaders["Water"], m_Textures["Water"], c_defaultWaterColor);
    m_Materials["SolidColor"] = new fw::Material(m_Shaders["SolidColor"], c_defaultObjColor);
    m_Materials["Arcade_Cabinet"] = new fw::Material(m_Shaders["Basic"], m_Textures["Arcade_Cabinet"], c_defaultObjColor);
    m_Materials["Arcade_Floor"] = new fw::Material(m_Shaders["Basic"], m_Textures["Arcade_Floor"], c_defaultObjColor);

    // Setup Scenes
    m_Scenes["Physics"] = new PhysicsScene(this);
    m_Scenes["Cube"] = new CubeScene(this);
    m_Scenes["Water"] = new WaterScene(this);
    m_Scenes["Obj"] = new ObjScene(this);

    SetCurrentScene(c_defaultScene);
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
        SetCurrentScene(pSceneChange->GetSceneName());
    }
    else
    {
		fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>(pEvent);

		if (pEvent->GetEventType() == fw::InputEvent::GetStaticEventType()
		&& (pInputEvent->GetDeviceType() == fw::DeviceType::Keyboard && pInputEvent->GetInputState() == fw::InputState::Input))
		{
			m_pImGuiManager->OnInput(pInputEvent);
		}
		else
		{
			m_pCurrentScene->OnEvent(pEvent);
		}
    }
}

void Game::Update(float deltaTime)
{
	MainMenu();

	if (m_showBGColorSelect)
	{
		BGColorSelect();
	}

	if (m_showDemo)
	{
		ImGui::ShowDemoWindow();
	}

    m_pCurrentScene->Update(deltaTime);
}

void Game::Draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_pCurrentScene->Draw();

    m_pImGuiManager->EndFrame();
}

void Game::SetCurrentScene(std::string scene)
{
	m_pCurrentScene = m_Scenes[scene];

	if (scene == "Obj")
	{
		if (m_backupColor == c_defaultBackground)
		{
			m_backgroundColor = Color4f::Black();
		}
	}
	else
	{
		m_backgroundColor = m_backupColor;
	}
		glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
}

void Game::ResetBackgroundColor(bool toBlack)
{
	m_backupColor = c_defaultBackground;

	if (toBlack)
	{
		m_backgroundColor = Color4f::Black();
	}
	else
	{
		m_backgroundColor = m_backupColor;
	}

	glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
}

void Game::BGColorSelect()
{
	ImGui::SetNextWindowSize(ImVec2(410, 300), ImGuiCond_Always);
	if (!ImGui::Begin("Background Color", &m_showBGColorSelect))
	{
		ImGui::End();
		return;
	}

	static bool alpha_preview = false;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	static bool hdr = false;
	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);


	static bool saved_palette_init = true;
	static fw::Color4f saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].r, saved_palette[n].g, saved_palette[n].b);
			saved_palette[n].a = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	static fw::Color4f backup_color = m_backgroundColor;

	ImGui::PushItemWidth(200.f);
	ImGui::ColorPicker4("##picker", (float*)&m_backgroundColor, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
	ImGui::SameLine();

	ImGui::BeginGroup(); // Lock X position
	ImGui::Text("Current");
	ImGui::ColorButton("##current", ImVec4(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
	ImGui::Text("Previous");
	if (ImGui::ColorButton("##previous", ImVec4(backup_color.r, backup_color.g, backup_color.b, backup_color.a), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
		m_backgroundColor = backup_color;
	ImGui::Separator();
	ImGui::Text("Palette");
	for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
	{
		ImGui::PushID(n);
		if ((n % 8) != 0)
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

		ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
		if (ImGui::ColorButton("##palette", ImVec4(saved_palette[n].r, saved_palette[n].g, saved_palette[n].b, saved_palette[n].a), palette_button_flags, ImVec2(20, 20)))
			m_backgroundColor = fw::Color4f(saved_palette[n].r, saved_palette[n].g, saved_palette[n].b, m_backgroundColor.a); // Preserve alpha!

		// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
		// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
				memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
				memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}
	ImGui::EndGroup();
	HelpMarker("This Window allows you to override the default Background Color.\n");

	if (m_backgroundColor.r != 0.0f && m_backgroundColor.g != 0.0f && m_backgroundColor.b != 0.0f)
	{
		m_backupColor = m_backgroundColor;
	}

	glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);

	ImGui::End();
}

void Game::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Load Scene"))
			{
				if (ImGui::MenuItem("Cube", "Ctrl+C"))
				{
					SceneChangeEvent* pSceneChange = new SceneChangeEvent("Cube");
					m_FWCore.GetEventManager()->AddEvent(pSceneChange);
				}

				if (ImGui::MenuItem("Obj File Loader", "Ctrl+L"))
				{
					SceneChangeEvent* pSceneChange = new SceneChangeEvent("Obj");
					m_FWCore.GetEventManager()->AddEvent(pSceneChange);
				}

				if (ImGui::MenuItem("Physics", "Ctrl+P"))
				{
					SceneChangeEvent* pSceneChange = new SceneChangeEvent("Physics");
					m_FWCore.GetEventManager()->AddEvent(pSceneChange);
				}

				if (ImGui::MenuItem("Water", "Ctrl+W"))
				{
					SceneChangeEvent* pSceneChange = new SceneChangeEvent("Water");
					m_FWCore.GetEventManager()->AddEvent(pSceneChange);
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Show ImGui Demo", "", &m_showDemo);
				ImGui::MenuItem("Enable Wireframe Mode", "", &m_wireframeToggle);

				if (m_wireframeToggle)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				ImGui::MenuItem("Change Background Color", "Ctrl+B", &m_showBGColorSelect);
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Quit", "Alt+F4")) { m_FWCore.Shutdown(); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Game::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
