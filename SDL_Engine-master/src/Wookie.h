#pragma once
#ifndef __WOOKIE__
#define __WOOKIE__

#include "DisplayObject.h"


class Wookie final : public DisplayObject {
public:
	Wookie();
	~Wookie();

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