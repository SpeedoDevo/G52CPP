#pragma once
#include "BaseEngine.h"
#include "Bxf03uStage.h"
#include "Bxf03uObstacle.h"
#include "Bxf03uTileManager.h"

class Bxf03uMain :
	public BaseEngine
{
friend class Bxf03uStage;
friend class Bxf03uTileManager;

public:
	Bxf03uMain();
	~Bxf03uMain();
	virtual int GameInit(void) override;
	void SetupBackgroundBuffer() override;

	virtual void KeyDown(int iKeyCode) override;
	virtual void KeyUp(int iKeyCode) override;

	virtual void GameAction() override;

	void DrawScreenRectangleFast(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour) const;

//for friend classes
protected:
	enum states { started = 0, over = 1, highscores = 2, running = 3};
	states state;
	std::string alphaStrokes;

private:
	int lastUpdate;

	Bxf03uStage stage;
	Bxf03uTileManager stateDisplay;

	virtual void DrawObjects() override;
	Bxf03uObstacle* FindClosestObstacle() const;
};

