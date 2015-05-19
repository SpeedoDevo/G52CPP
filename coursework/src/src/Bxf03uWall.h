#pragma once
#include "header.h"
#include "Bxf03uMain.h"
#include "DisplayableObject.h"

class Bxf03uStage;

class Bxf03uWall :
	public DisplayableObject
{
public:
	Bxf03uWall(BaseEngine* engine, Bxf03uStage* stage, bool onLeft);
	~Bxf03uWall();

	virtual void Draw() override;
private:
	bool onLeft;
	Bxf03uMain* engine;
	Bxf03uStage* stage;
};

