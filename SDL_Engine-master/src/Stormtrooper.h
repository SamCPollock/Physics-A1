#pragma once
#ifndef __STORMTROOPER__
#define __STORMTROOPER__

#include "DisplayObject.h"


class Stormtrooper final : public DisplayObject {
public:
	Stormtrooper();
	~Stormtrooper();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;


private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */