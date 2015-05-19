#pragma once
#include "header.h"
#include "JPGImage.h"
#include "BaseEngine.h"
#include "BXf03uPlayer.h"

class Bxf03uStage
{
public:
	Bxf03uStage();
	~Bxf03uStage();
	
	void init(Bxf03uMain* engine);
	void update(int time);
	void draw();
	void over();
	
	//coordinate conversions
	inline int Bxf03uStage::toActualX(float x)
	{
		return x + Xoffset;
	}
	inline int Bxf03uStage::toActualY(float y)
	{
		return y + Yoffset;
	}
	inline float Bxf03uStage::toStageX(int x)
	{
		return x - Xoffset;
	}
	inline float Bxf03uStage::toStageY(int y)
	{
		return y - Yoffset;
	}

	int score;
	
	Bxf03uMain* engine;
	Bxf03uPlayer* player;

private:
	float bgX;
	float bgY;
	float Xoffset;
	float Yoffset;
	float lastObstacleY;

	bool godMode;

	ImageData bg;
	Bxf03uHook* hook;
	std::deque<DisplayableObject*>::iterator firstObstacle;

	void GenerateNewObstacle();
};

