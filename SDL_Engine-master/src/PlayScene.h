#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Target.h"
#include "Wookie.h"
#include "Stormtrooper.h"
#include "SimulationStats.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	SimulationStats m_sim;
	float floor;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Target* m_pBall;
	//Player* m_pPlayer;
	Wookie* m_pWookie;
	Stormtrooper* m_pStormtrooper;

	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_pRangeLabel;
	Label* m_pHeightLabel;
	Label* m_pVelocityLabel;
	Label* m_pStatsLabel;

};

#endif /* defined (__PLAY_SCENE__) */