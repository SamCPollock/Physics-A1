#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include <sStream>
#include <iomanip>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	std::ostringstream out;
	out.precision(2);
	out << std::fixed << std::to_string(m_pBall->getTransform()->position.x);
	updateDisplayList();


	// Updating the label content
	std::string labelText = "";


	// set value for RangeLabel
	labelText = out.str();

	//glm::vec2 vel = m_pBall->getRigidBody()->velocity;
	//float magnitude = Util::magnitude(vel);
	m_pRangeLabel->setText(labelText);

	// set value for HeightLabel
	labelText = std::to_string(m_pBall->getTransform()->position.y);
	//glm::vec2 vel = m_pBall->getRigidBody()->velocity;
	//float magnitude = Util::magnitude(vel);
	m_pHeightLabel->setText(labelText);

	// set value for VelocityLabel
	float magnitude = Util::magnitude(m_pBall->getRigidBody()->velocity);
	labelText = std::to_string(magnitude);
	m_pVelocityLabel->setText(labelText);

	

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	


	// handle player movement if no Game Controllers found
	
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
	TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };

	// Range Label
	m_pRangeLabel = new Label("Position", "Consolas", 40, blue, glm::vec2(400.0f, 40.0f));
	m_pRangeLabel->setParent(this);
	addChild(m_pRangeLabel);

	// Height Label
	m_pHeightLabel = new Label("Position", "Consolas", 40, blue, glm::vec2(400.0f, 80.0f));
	m_pHeightLabel->setParent(this);
	addChild(m_pHeightLabel);

	// Velocity
	m_pVelocityLabel = new Label("Velocity", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pVelocityLabel->setParent(this);
	addChild(m_pVelocityLabel);

	// Loads the background texture from disk to ram
	TextureManager::Instance()->load("../Assets/textures/Tatooine.jpg", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	
	
	// Wookie
	m_pWookie = new Wookie();
	addChild(m_pWookie);
	//m_pWookie->getTransform()->scale = glm::vec2(0.1f, 0.1f);

	// Ball
	m_pBall = new Target();
	addChild(m_pBall);

	//// Stormtrooper
	m_pStormtrooper = new Stormtrooper();
	addChild(m_pStormtrooper);

	// Player Sprite
	//m_pPlayer = new Player();
	//addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
		{
			m_pBackButton->setActive(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pBackButton->setAlpha(128);
		});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pBackButton->setAlpha(255);
		});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
		{
			m_pNextButton->setActive(false);
			TheGame::Instance()->changeSceneState(END_SCENE);
		});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pNextButton->setAlpha(128);
		});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pNextButton->setAlpha(255);
		});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Throw"))
	{
		m_pBall->throwPosition = m_pWookie->getTransform()->position;
		m_pBall->doThrow();

		std::cout << "Throw Pressed" << std::endl;
	}

	ImGui::Separator();

	// To do: Figure out why this check box isn't working. 

	static bool gravityEnabled = false;
	if (ImGui::Checkbox("Gravity", &gravityEnabled))
	{
		m_pBall->gravityEnabled = gravityEnabled;
	}

	// Angle slider
	static float throwingAngle = 0;
	if (ImGui::SliderFloat("Throw Angle", &throwingAngle, 0, 180))
	{
		m_pBall->throwAngle = -throwingAngle;
	}
	

	// Velocity slider
	static float throwingVelocity = 0;
	if (ImGui::SliderFloat("throw velocity", &throwingVelocity, 0, 100))
	{
		m_pBall->throwSpeed = throwingVelocity;
	}

		


	static int xPlayerPos = 200;
	if (ImGui::SliderInt("Player xPosition", &xPlayerPos, 0, 800))
	{
		m_pWookie->getTransform()->position.x = xPlayerPos;
		//m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBall->getTransform()->position = glm::vec2(xPlayerPos, 300);
	}

	/*static float velocity[2] = { 0 , 0 };
	if (ImGui::SliderFloat2("Throw Speed", velocity, 0, 50))
	{
		m_pBall->throwSpeed = glm::vec2(velocity[0], -velocity[1]);
	}*/

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
