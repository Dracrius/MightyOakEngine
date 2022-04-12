#include "Framework.h"
#include "DefaultSettings.h"

#include "Game.h"
#include "GameEvents/GameEvents.h"
#include "Meshes/Shapes.h"
#include "Scenes/Assignment1Scene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/ObjScene.h"
#include "Scenes/Physics3DScene.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/ThirdPersonScene.h"
#include "Scenes/WaterScene.h"

Game::Game(fw::FWCore& fwCore)
    : m_FWCore( fwCore )
{
    Init();
}

Game::~Game()
{
	m_FWCore.GetEventManager()->UnregisterForEvents(fw::InputEvent::GetStaticEventType(), this);
	m_FWCore.GetEventManager()->UnregisterForEvents(SceneChangeEvent::GetStaticEventType(), this);

	for (auto& it : m_Scenes)
    {
        delete it.second;
    }

    delete m_pImGuiManager;
}

void Game::Init()
{
	m_FWCore.GetEventManager()->RegisterForEvents(fw::InputEvent::GetStaticEventType(), this);
	m_FWCore.GetEventManager()->RegisterForEvents(SceneChangeEvent::GetStaticEventType(), this);

    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore );

    // OpenGL settings.
	glViewport((c_windowSize.x - c_glRenderSize.x) / 2, (c_windowSize.y - c_glRenderSize.y) / 2, c_glRenderSize.x, c_glRenderSize.y);

    glPointSize( 10 );

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glFrontFace(GL_CW);

    // Setup Frame Budffer
    m_pOffScreenFBO = new fw::FrameBufferObject(c_glRenderSize.x, c_glRenderSize.y, { fw::FrameBufferObject::FBOColorFormat_RGBA_UByte });

    // Setup Meshes 
	m_pResourceManager->CreateMesh("Sprite", GL_TRIANGLES, g_SpriteVerts, g_SpriteIndices);
	m_pResourceManager->CreateMesh("Background");
	m_pResourceManager->GetMesh("Background")->CreatePlane(vec2(10.f, 2.f), ivec2(2, 2));
	m_pResourceManager->CreateMesh("Platform", GL_TRIANGLES, g_BackgroundVerts, g_BackgroundIndices);
    m_pResourceManager->CreateMesh("Cube", GL_TRIANGLES, g_CubeVerts);
	m_pResourceManager->CreateMesh("Plane");
	m_pResourceManager->GetMesh("Plane")->CreatePlane(vec2(100.f, 100.f), ivec2(1000, 1000));
	m_pResourceManager->CreateMesh("Obj");
    m_pResourceManager->CreateMesh("Facehugger");
    m_pResourceManager->GetMesh("Facehugger")->LoadObj("Data/Models/Chibi_Facehugger.obj", true);
	m_pResourceManager->CreateMesh("Sphere");
	m_pResourceManager->GetMesh("Sphere")->LoadObj("Data/Models/sphere.obj", true);


    // Setup Shaders
	m_pResourceManager->CreateShader("Basic", "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag");
	m_pResourceManager->CreateShader("Water", "Data/Shaders/Water.vert", "Data/Shaders/Water.frag");
	m_pResourceManager->CreateShader("SolidColor", "Data/Shaders/SolidColor.vert", "Data/Shaders/SolidColor.frag");
	m_pResourceManager->CreateShader("Lit-Color", "Data/Shaders/Light-SolidColor.vert", "Data/Shaders/Light-SolidColor.frag");

    // Setup Textures
	m_pResourceManager->CreateTexture("Sprites", "Data/Textures/Sprites.png");
	m_pResourceManager->CreateTexture("Cube", "Data/Textures/CubeTexture.png");
	m_pResourceManager->CreateTexture("Water", "Data/Textures/WaterTile.png");
	m_pResourceManager->CreateTexture("Arcade_Cabinet", "Data/Textures/Arcade_Cabinet.png");
	m_pResourceManager->CreateTexture("Arcade_Floor", "Data/Textures/Arcade_Cabinet_Floor_Low_Light.png");
	m_pResourceManager->CreateTexture("Background", "Data/Textures/mayclover_meadow.png");
	m_pResourceManager->CreateTexture("NiceDaysWalk", "Data/Textures/NiceDaysWalk.png");
	m_pResourceManager->CreateTexture("PlatformCenter", "Data/Textures/Ground_02.png");

    // Setup Sprite Sheets
	m_pResourceManager->CreateSpriteSheet("Sprites", "Data/Textures/Sprites.json", m_pResourceManager->GetTexture("Sprites"));
	m_pResourceManager->CreateSpriteSheet("NiceDaysWalk", "Data/Textures/NiceDaysWalk.json", m_pResourceManager->GetTexture("NiceDaysWalk"));

    // Setup Materials
	m_pResourceManager->CreateMaterial("SolidColor", m_pResourceManager->GetShader("SolidColor"), c_defaultObjColor);
    m_pResourceManager->CreateMaterial("Red", m_pResourceManager->GetShader("SolidColor"), fw::Color4f::Red());
    m_pResourceManager->CreateMaterial("Green", m_pResourceManager->GetShader("SolidColor"), fw::Color4f::Green());
    m_pResourceManager->CreateMaterial("Blue", m_pResourceManager->GetShader("SolidColor"), fw::Color4f::Blue());
    m_pResourceManager->CreateMaterial("White", m_pResourceManager->GetShader("SolidColor"), fw::Color4f::White());

	m_pResourceManager->CreateMaterial("Lit-SolidColor", m_pResourceManager->GetShader("Lit-Color"), c_defaultObjColor);
	m_pResourceManager->CreateMaterial("Lit-Red", m_pResourceManager->GetShader("Lit-Color"), fw::Color4f::Red());
	m_pResourceManager->CreateMaterial("Lit-Green", m_pResourceManager->GetShader("Lit-Color"), fw::Color4f::Green());
	m_pResourceManager->CreateMaterial("Lit-Blue", m_pResourceManager->GetShader("Lit-Color"), fw::Color4f::Blue());
	m_pResourceManager->CreateMaterial("Lit-White", m_pResourceManager->GetShader("Lit-Color"), fw::Color4f::White());

	m_pResourceManager->CreateMaterial("Sokoban", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Sprites"), fw::Color4f::Red());
	m_pResourceManager->CreateMaterial("Cube", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Cube"), fw::Color4f::Green());
	m_pResourceManager->CreateMaterial("Water", m_pResourceManager->GetShader("Water"), m_pResourceManager->GetTexture("Water"), c_defaultWaterColor);
	m_pResourceManager->CreateMaterial("Arcade_Cabinet", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Arcade_Cabinet"), c_defaultObjColor);
	m_pResourceManager->CreateMaterial("Arcade_Floor", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Arcade_Floor"), c_defaultObjColor);
	m_pResourceManager->CreateMaterial("Background", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Background"), c_defaultWaterColor);
	m_pResourceManager->CreateMaterial("NiceDaysWalk", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("NiceDaysWalk"), fw::Color4f::Red());
	m_pResourceManager->CreateMaterial("PlatformCenter", m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("PlatformCenter"), fw::Color4f::Red());

    // Setup Scenes
    m_Scenes["Physics"] = new PhysicsScene(this);
    m_Scenes["Cube"] = new CubeScene(this);
    m_Scenes["Water"] = new WaterScene(this);
    m_Scenes["Obj"] = new ObjScene(this);
	m_Scenes["ThirdPerson"] = new ThirdPersonScene(this);
	m_Scenes["Assignment1"] = new Assignment1Scene(this);
    m_Scenes["Physics3D"] = new Physics3DScene(this);

    SetCurrentScene(c_defaultScene);
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking;
	flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::Begin("Main Dock", nullptr, flags);
	ImGui::DockSpace(ImGui::GetID("Game Dockspace"));
	ImGui::End();

    m_pCurrentScene->StartFrame(deltaTime);
}

void Game::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetEventType() == "SceneChangeEvent")
    {
        SceneChangeEvent* pSceneChange = static_cast<SceneChangeEvent*>(pEvent);
        SetCurrentScene(pSceneChange->GetSceneName());

		if (pSceneChange->GetSceneName() == "Assignment1")
		{
			static_cast<Assignment1Scene*>(m_Scenes["Assignment1"])->ReloadScene();
		}
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
    // Off-Screen
    m_pOffScreenFBO->Bind();
    glViewport(0, 0, m_pOffScreenFBO->GetRequestedWidth(), m_pOffScreenFBO->GetRequestedHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pCurrentScene->Draw();
    m_pOffScreenFBO->Unbind();

    // On-Screen
    glViewport(0,0, c_windowSize.x, c_windowSize.y);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    ImGui::Begin("Scene");
	ImVec2 windowMin = ImGui::GetWindowContentRegionMin();
	ImVec2 windowMax = ImGui::GetWindowContentRegionMax();
	ImVec2 windowSize(windowMax.x - windowMin.x, windowMax.y - windowMin.y);

	m_pCurrentScene->GetCamera()->SetAspectRatio(windowSize.x / windowSize.y);
	m_pOffScreenFBO->Resize(windowSize.x, windowSize.y);

    ImTextureID textureID = (ImTextureID)(intptr_t)m_pOffScreenFBO->GetColorTextureHandle(0);

	ImVec2 uv0 = ImVec2(0, m_pOffScreenFBO->GetHeightRatio());
	ImVec2 uv1 = ImVec2(m_pOffScreenFBO->GetWidthRatio(), 0);

    ImGui::Image(textureID, windowSize, uv0, uv1);
    ImGui::End();

    m_pImGuiManager->EndFrame();
}

void Game::SetCurrentScene(std::string scene)
{
	m_pCurrentScene = m_Scenes[scene];

	if (scene == "Obj" || scene == "ThirdPerson")
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
				if (ImGui::MenuItem("Assignment 1", "Ctrl+A"))
				{
					SceneChangeEvent* pSceneChange = new SceneChangeEvent("Assignment1");
					m_FWCore.GetEventManager()->AddEvent(pSceneChange);
				}

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

                if (ImGui::MenuItem("Physics3D", ""))
                {
                    SceneChangeEvent* pSceneChange = new SceneChangeEvent("Physics3D");
                    m_FWCore.GetEventManager()->AddEvent(pSceneChange);
                }

                if (ImGui::MenuItem("ThirdPerson", ""))
                {
                    SceneChangeEvent* pSceneChange = new SceneChangeEvent("ThirdPerson");
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
