#pragma once
#include "BXf03uPlayer.h"
#include "Bxf03uObstacle.h"
#include "DisplayableObject.h"

class Bxf03uHook :
	public DisplayableObject
{
friend class Bxf03uPlayer;

public:
	Bxf03uHook(BaseEngine* engine, Bxf03uStage* stage, Bxf03uPlayer* player);
	~Bxf03uHook();

	virtual void Draw() override;

	virtual void DoUpdate(int iCurrentTime) override;

//only intended for the player
protected:
	void HookOn(Bxf03uObstacle* obstacle);
	void HookOff();

private:
	int radius;
	int m_iCurrentScreenX2;
	int m_iCurrentScreenY2;

	Bxf03uStage* stage;
	Bxf03uPlayer* player;
	Bxf03uObstacle* obstacle;
};

