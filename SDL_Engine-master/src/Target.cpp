#include "Target.h"
#include "TextureManager.h"
#include "glm/glm.hpp"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/thermalDetonator.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 350);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	if(simStarted)
	{
		float deltaTime = 1.0f / 60 * timeScale;
		glm::vec2 gravityvec = glm::vec2(0, gravity);
		float metersPerPixel = 1;


		if(gravityEnabled)
		{
			getRigidBody()->velocity += (getRigidBody()->acceleration + gravityvec) * deltaTime;
		}
		else
		{
			getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
		}

		getTransform()->position += (getRigidBody()->velocity * metersPerPixel) * deltaTime; // actually move it
	}
}

void Target::m_checkBounds()
{
	if(getTransform()->position.y >= floor)
	{
		simStarted = false;
	}
}

void Target::m_reset()
{
}

void Target::doThrow()
{
	simStarted = true;
	getRigidBody()->velocity = glm::vec2(cos(glm::radians(throwAngle)), sin(glm::radians(throwAngle)));
	getRigidBody()->velocity *= throwSpeed;
	getTransform()->position = throwPosition;
}

void Target::setTimescale(float scale)
{
	timeScale = scale;
}


