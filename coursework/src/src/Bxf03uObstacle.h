#pragma once
#include "DisplayableObject.h"

class Bxf03uStage;

class Bxf03uObstacle :
	public DisplayableObject
{
friend class Bxf03uHook;
friend class Bxf03uMain;
friend class Bxf03uPlayer;

public:
	Bxf03uObstacle(BaseEngine* engine, Bxf03uStage* stage, int stageX, int stageY, int diameter);
	~Bxf03uObstacle();

	virtual void Draw() override;

	virtual void DoUpdate(int iCurrentTime) override;

//only intended for friends
protected:
	const int stageX;
	const int stageY;
	float distanceToPlayer;

	void HookOn();

private:
	int x1;
	int y1;
	int x2;
	int y2;
	int radius;
	bool onScreen;
	
	float r;
	float g;
	float b;
	float gSpeed;
	float bSpeed;
	float rSpeed;
	unsigned int color;
	bool transitionFinished;
	
	Bxf03uStage* stage;
	
	float FindDistanceTo(int x, int y);
};

