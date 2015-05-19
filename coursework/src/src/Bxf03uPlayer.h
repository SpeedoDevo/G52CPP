#pragma once
#include "header.h"
#include "Bxf03uHook.h"
#include "Bxf03uObstacle.h"
#include "DisplayableObject.h"

//breaking a circular reference
class Bxf03uStage;

class Bxf03uPlayer :
	public DisplayableObject
{

friend class Bxf03uMain;
friend class Bxf03uHook;
friend class Bxf03uWall;
friend class Bxf03uStage;
friend class Bxf03uObstacle;

public:
	Bxf03uPlayer(BaseEngine* engine, Bxf03uStage* stage, Bxf03uHook* hook);
	~Bxf03uPlayer();

	virtual void Draw() override;

	virtual void DoUpdate(int iCurrentTime) override;

	void HookOn(Bxf03uObstacle* hookCentre);
	void HookOff();

//this is to say that friend classes should only access these
protected:
	int radius;
	bool hooked;
	float stageX;
	float stageY;
	bool waitForHook;
	Bxf03uObstacle* hookCentre;

private:
	int hookX;
	int hookY;
	int lastUpdate;
	float speed;
	float direction;
	float angularVelocity;
	Bxf03uHook* hook;
	Bxf03uStage* stage;
	float GetVectorAngle(float x1, float y1, float x2, float y2) const;
};

