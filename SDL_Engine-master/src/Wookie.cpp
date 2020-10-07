#include "Wookie.h"
#include "TextureManager.h"

Wookie::Wookie()
{
	TextureManager::Instance()->load("../Assets/textures/wookieSmall.png","wookie");
	
	const auto size = TextureManager::Instance()->getTextureSize("wookie");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->scale = glm::vec2(10, 10);
	getTransform()->position = glm::vec2(100.0f, 350);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//setType(WOOKIE);
}

Wookie::~Wookie()
	= default;


void Wookie::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("wookie", x, y, 0, 255, true);
}

void Wookie::update()
{
	m_move();
	m_checkBounds();
}

void Wookie::clean()
{
}

void Wookie::m_move()
{
}

void Wookie::m_checkBounds()
{
}

void Wookie::m_reset()
{
}
