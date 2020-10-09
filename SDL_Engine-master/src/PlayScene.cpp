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
	
	if(m_sim.getGravity() > 0.0f && m_sim.gravityEnabled)
	{
		float increment = 0.075f * m_sim.getTimescale();
		
		if(m_sim.showAltTrajectory)
		{
			// Draw Secondary Trajectory
			SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 192, 192, 192, 255);
			for(float t = 0; t < m_sim.getAltTime(); t += increment)
			{
				glm::vec2 pos = m_sim.getAltDisplacementAtTime(t);
				pos.x = m_pWookie->getTransform()->position.x + pos.x;
				pos.y = m_pWookie->getTransform()->position.y - pos.y;
				//if(pos.y < floor)
				SDL_RenderDrawPointF(Renderer::Instance()->getRenderer(), pos.x, pos.y);
			}
		}

		if(m_sim.showTrajectory)
		{
			// Draw Primary Trajectory
			SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
			for(float t = 0; t < m_sim.getTime(); t += increment)
			{
				glm::vec2 pos = m_sim.getDisplacementAtTime(t);
				pos.x = m_pWookie->getTransform()->position.x + pos.x;
				pos.y = m_pWookie->getTransform()->position.y - pos.y;
				//if(pos.y < floor)
				SDL_RenderDrawPointF(Renderer::Instance()->getRenderer(), pos.x, pos.y);
			}
		}
	}

	drawDisplayList();

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	std::ostringstream out;
	out.precision(2);
	
	updateDisplayList();

	auto wookiePosition = m_pWookie->getTransform()->position; 
	

	// Updating the label content


	// set value for RangeLabel
	out << std::fixed << "Range: " << m_pBall->getTransform()->position.x - wookiePosition.x<< "m";
	m_pRangeLabel->setText(out.str());

	// set value for HeightLabel
	out.str("");
	out.clear();
	out << std::fixed << "Height: "<< - m_pBall->getTransform()->position.y + wookiePosition.y << "m";
	m_pHeightLabel->setText(out.str());

	// set value for VelocityLabel
	out.str("");
	out.clear();
	float magnitude = Util::magnitude(m_pBall->getRigidBody()->velocity);
	out << std::fixed <<"Speed: " << magnitude << "m/s^2";
	m_pVelocityLabel->setText(out.str());

	out.str("");
	out.clear();
	out << std::fixed << "Time: " << m_sim.getTime() << "s";
	m_pStatsLabel->setText(out.str());
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

	// Range label
	m_pRangeLabel = new Label("Position", "Consolas", 40, blue, glm::vec2(400.0f, 40.0f));
	m_pRangeLabel->setParent(this);
	addChild(m_pRangeLabel);

	// Height label
	m_pHeightLabel = new Label("Position", "Consolas", 40, blue, glm::vec2(400.0f, 80.0f));
	m_pHeightLabel->setParent(this);
	addChild(m_pHeightLabel);

	// Velocity label
	m_pVelocityLabel = new Label("Velocity", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pVelocityLabel->setParent(this);
	addChild(m_pVelocityLabel);

	// Stats label
	m_pStatsLabel = new Label("Stats", "Consolas", 20, blue, glm::vec2(40.0f, 40.0f));
	m_pStatsLabel->setParent(this);
	addChild(m_pStatsLabel);

	// Loads the background texture from disk to ram
	TextureManager::Instance()->load("../Assets/textures/Tatooine.jpg", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	floor = 350;

	//// Stormtrooper
	m_pStormtrooper = new Stormtrooper();
	addChild(m_pStormtrooper);
	
	// Ball
	m_pBall = new Target();
	m_pBall->floor = floor;
	m_pBall->gravityEnabled = m_sim.gravityEnabled;
	m_pBall->gravity = m_sim.getGravity();
	m_pBall->throwAngle = -m_sim.getAngle();
	m_pBall->throwSpeed = m_sim.getSpeed();
	m_pBall->getRigidBody()->velocity = m_sim.getVelocity();
	addChild(m_pBall);

	// Wookie
	m_pWookie = new Wookie();
	addChild(m_pWookie);
	m_pBall->throwPosition = m_pWookie->getTransform()->position;
	m_pBall->getTransform()->position = m_pWookie->getTransform()->position;

	// Player Sprite
	//m_pPlayer = new Player();
	//addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Back Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	//m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//	{
	//		m_pBackButton->setActive(false);
	//		TheGame::Instance()->changeSceneState(START_SCENE);
	//	});

	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//	{
	//		m_pBackButton->setAlpha(128);
	//	});

	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//	{
	//		m_pBackButton->setAlpha(255);
	//	});
	//addChild(m_pBackButton);

	//// Next Button
	//m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	//m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	//m_pNextButton->addEventListener(CLICK, [&]()-> void
	//	{
	//		m_pNextButton->setActive(false);
	//		TheGame::Instance()->changeSceneState(END_SCENE);
	//	});

	//m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	//	{
	//		m_pNextButton->setAlpha(128);
	//	});

	//m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	//	{
	//		m_pNextButton->setAlpha(255);
	//	});

	//addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float throwingAngle = m_sim.getAngle();
	static float throwingVelocity = m_sim.getSpeed();
	static float gravitySlider = m_sim.getGravity();
	static float rangeSlider = m_sim.getRange();
	static float timeSlider = m_sim.getTime();
	static int xPlayerPos = 100;
	static int xStormtrooperPos = xPlayerPos + m_sim.getRange();
	static int primaryDependentVariable = static_cast<int>(m_sim.primaryVariable);
	static int secondaryDependentVariable = static_cast<int>(m_sim.secondaryVariable);

	if (ImGui::Button("Throw"))
	{
		m_pBall->throwPosition = m_pWookie->getTransform()->position;
		m_pBall->doThrow();

		std::cout << "Throw Pressed" << std::endl;
	}
	ImGui::SameLine();
	if(ImGui::Button("Reset"))
	{
		m_sim.ResetToDefaults();
		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();

		m_pBall->gravityEnabled = m_sim.gravityEnabled;
		m_pBall->throwAngle = -m_sim.getAngle();
		m_pBall->throwSpeed = m_sim.getSpeed();
		m_pBall->getRigidBody()->velocity = m_sim.getVelocity();
		m_pBall->getTransform()->position = m_pWookie->getTransform()->position;
		primaryDependentVariable = static_cast<int>(m_sim.primaryVariable);
		secondaryDependentVariable = static_cast<int>(m_sim.secondaryVariable);
	}

	ImGui::Separator();

	if (ImGui::Checkbox("Gravity", &m_sim.gravityEnabled))
	{
		m_pBall->gravityEnabled = m_sim.gravityEnabled;
	}
	ImGui::SameLine();
	ImGui::Checkbox("Trajectory", &m_sim.showTrajectory);
	ImGui::SameLine();
	ImGui::Checkbox("Complementary", &m_sim.showAltTrajectory);
	ImGui::SameLine();
	ImGui::Checkbox("Stats", &m_sim.showStats);

	ImGui::Separator();
	ImGui::Text("Primary Dependent Variable");
	if(ImGui::RadioButton("Range##r1", &primaryDependentVariable, 0))		{ m_sim.primaryVariable = static_cast<SimulationStats::variable>(primaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Speed##s1", &primaryDependentVariable, 1))		{ m_sim.primaryVariable = static_cast<SimulationStats::variable>(primaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Angle##a1", &primaryDependentVariable, 2))		{ m_sim.primaryVariable = static_cast<SimulationStats::variable>(primaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Gravity##g1", &primaryDependentVariable, 3))		{ m_sim.primaryVariable = static_cast<SimulationStats::variable>(primaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Time##t1", &primaryDependentVariable, 4))		{ m_sim.primaryVariable = static_cast<SimulationStats::variable>(primaryDependentVariable); };
	ImGui::Text("Secondary Dependent Variable");
	if(ImGui::RadioButton("Range##r2", &secondaryDependentVariable, 0))		{ m_sim.secondaryVariable = static_cast<SimulationStats::variable>(secondaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Speed##s2", &secondaryDependentVariable, 1))		{ m_sim.secondaryVariable = static_cast<SimulationStats::variable>(secondaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Angle##a2", &secondaryDependentVariable, 2))		{ m_sim.secondaryVariable = static_cast<SimulationStats::variable>(secondaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Gravity##g2", &secondaryDependentVariable, 3))	{ m_sim.secondaryVariable = static_cast<SimulationStats::variable>(secondaryDependentVariable); }; ImGui::SameLine();
	if(ImGui::RadioButton("Time##t2", &secondaryDependentVariable, 4))		{ m_sim.secondaryVariable = static_cast<SimulationStats::variable>(secondaryDependentVariable); }; 

	// Angle slider
	if (ImGui::SliderFloat("Throw Angle", &throwingAngle, 0, 180))
	{
		m_sim.setAngle(throwingAngle);
		m_pBall->throwAngle = -throwingAngle;
		
		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();
		rangeSlider = m_sim.getRange();
		timeSlider = m_sim.getTime();
	}

	// Velocity slider
	if (ImGui::SliderFloat("Throw Velocity", &throwingVelocity, 0, 500))
	{
		m_sim.setSpeed(throwingVelocity);
		m_pBall->throwSpeed = throwingVelocity;

		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();
		rangeSlider = m_sim.getRange();
		timeSlider = m_sim.getTime();
	}

	// Velocity slider
	if(ImGui::SliderFloat("Simulation Gravity", &gravitySlider, 0.1, 100))
	{
		m_sim.setGravity(gravitySlider);
		m_pBall->gravity = m_sim.getGravity();

		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();
		rangeSlider = m_sim.getRange();
		timeSlider = m_sim.getTime();
	}

	// Range slider
	if(ImGui::SliderFloat("Target Range", &rangeSlider, 0.1, 1000))
	{
		m_sim.setRange(rangeSlider);

		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();
		rangeSlider = m_sim.getRange();
		timeSlider = m_sim.getTime();
	}

	// Time slider
	if(ImGui::SliderFloat("Target Time", &timeSlider, 0.1, 1000))
	{
		m_sim.setTime(timeSlider);

		throwingAngle = m_sim.getAngle();
		throwingVelocity = m_sim.getSpeed();
		gravitySlider = m_sim.getGravity();
		rangeSlider = m_sim.getRange();
		timeSlider = m_sim.getTime();
	}

	if (ImGui::SliderInt("Player Position", &xPlayerPos, 0, 800))
	{
		m_pWookie->getTransform()->position.x = xPlayerPos;
		//m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBall->getTransform()->position = glm::vec2(xPlayerPos, floor);
	}

	if (ImGui::SliderInt("Stormtrooper Position", &xStormtrooperPos, 0, 800))
	{
		m_pStormtrooper->getTransform()->position.x = xStormtrooperPos;
	}

	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
