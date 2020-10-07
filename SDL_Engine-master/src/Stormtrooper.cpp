#include "Stormtrooper.h"
#include "TextureManager.h"

Stormtrooper::Stormtrooper()
{
	TextureManager::Instance()->load("../Assets/textures/stormtrooperSmall.png", "stormtrooper");

	const auto size = TextureManager::Instance()->getTextureSize("stormtrooper");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(400.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//setType(Stormtrooper);
}

Stormtrooper::~Stormtrooper()
= default;


void Stormtrooper::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("stormtrooper", x, y, 0, 255, true);
}

void Stormtrooper::update()
{
	m_move();
	m_checkBounds();
}

void Stormtrooper::clean()
{
}

void Stormtrooper::m_move()
{
}

void Stormtrooper::m_checkBounds()
{
}

void Stormtrooper::m_reset()
{
}
