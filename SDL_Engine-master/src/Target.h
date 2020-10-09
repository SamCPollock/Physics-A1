#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"


class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void doThrow();

	bool gravityEnabled = false;
	float gravity;
	bool simStarted = false;
	float floor;
	float throwSpeed;
	glm::vec2 throwPosition;
	float throwAngle;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */