#include "Target.h"
#include "TextureManager.h"
#include "glm/glm.hpp"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/thermalDetonator.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
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
	float deltaTime = 1.0f / 60;
	glm::vec2 gravity = glm::vec2(0, 9.8);
	float metersPerPixel = 30;


	if (gravityEnabled)
	{
		getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
	}
	else
	{
		getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	}

	getTransform()->position += (getRigidBody()->velocity * metersPerPixel) * deltaTime; // actually move it

}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

void Target::doThrow()
{
	getRigidBody()->velocity = glm::vec2(cos(glm::radians(throwAngle)), sin(glm::radians(throwAngle)));
	getRigidBody()->velocity *= throwSpeed;
	getTransform()->position = throwPosition;
}
