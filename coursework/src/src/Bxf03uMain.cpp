#include <string>
#include <iostream>
#include "header.h"
#include "templates.h"
#include "Bxf03uMain.h"
#include "FontManager.h"


Bxf03uMain::Bxf03uMain()
	: BaseEngine(0)
	, stateDisplay(Bxf03uTileManager(500, 700, &stage))
{
}

Bxf03uMain::~Bxf03uMain()
{
}

int Bxf03uMain::GameInit(void)
{

	// Set up the initial background
	SetupBackgroundBuffer();

	//set state
	state = started;
	
	//initailise stage, will remove old objects
	stage.init(this);
	stateDisplay.InitFonts();

	//ask for one update
	stage.update(GetTime());
	//draw that update
	stage.draw();
	//ask for a redraw
	Redraw(true);

	return 0;
}

void Bxf03uMain::SetupBackgroundBuffer()
{
	//not really needed the image background overwrites it
	FillBackground(0x333333);
}

//delegate key events
void Bxf03uMain::KeyDown(int iKeyCode)
{
	//each state has a keymapping
	switch (state)
	{
	case Bxf03uMain::started:
		//space starts
		if (iKeyCode == SDLK_SPACE)
		{
			state = running;
			Redraw(true);
		}
		//n shows the highscores
		else if (iKeyCode == SDLK_n)
		{
			state = highscores;
			//state change, ask for a redraw
			Redraw(true);
		}
		break;
	case Bxf03uMain::running:
		//space hooks
		if (iKeyCode == SDLK_SPACE)
		{
			if (stage.player->hooked || stage.player->waitForHook) return;
			auto obstacle = FindClosestObstacle();
  			stage.player->HookOn(obstacle);
		}
		//m restarts
		else if (iKeyCode == SDLK_m)
		{
			stage.init(this);
			stateDisplay.reset();
		}

		if (SDLK_a <= iKeyCode && iKeyCode <= SDLK_z)
		{
			alphaStrokes.push_back((char)iKeyCode);
		}
		break;
	case Bxf03uMain::over:
		//m restarts
		if (iKeyCode == SDLK_m)
		{
			stage.init(this);
			state = running;
		}
		//n shows the highscores
		else if (iKeyCode == SDLK_n)
		{
			state = highscores;
			//state change, ask for a redraw
			Redraw(true);
		}
		break;
	case Bxf03uMain::highscores:
		//m restarts
		if (iKeyCode == SDLK_m)
		{
			stage.init(this);
			state = running;
		}
		break;
	}

	//esc quits anywhere
	if (iKeyCode == SDLK_ESCAPE)
	{
		SetExitWithCode(0);
	}
}

//find the closest obstacle for the player for hoking
Bxf03uObstacle* Bxf03uMain::FindClosestObstacle() const
{
	Bxf03uObstacle* obstacle;
	int minDistance = INT_MAX;
	if (&DisplayableObjects != NULL)
	{
		//loop through the objects
		for each (DisplayableObject* ob in DisplayableObjects)
		{
			//stop if NULL
			if (ob == NULL) break;
			auto obs = dynamic_cast<Bxf03uObstacle*>(ob);
			//don't even try if it's not an obstacle
			if (obs == NULL) continue;
			//calculate minimum distance
			int distance = obs->distanceToPlayer;
			if (distance < minDistance) {
				obstacle = obs;
				minDistance = distance;
			}
		}
	}
	return obstacle;
}

void Bxf03uMain::KeyUp(int iKeyCode)
{
	//release hook
	if (iKeyCode == SDLK_SPACE && state == running)
	{
		stage.player->HookOff();
	}
}




void Bxf03uMain::GameAction()
{
	//no update if the game isn't running
	if (!IsTimeToActWithSleep() || state != running)
		return;
	
	// save this as the last update
	lastUpdate = GetTime();
	stage.update(lastUpdate);

	//"limit" framerate to 300
	int sleep = 1000 / 300 - (GetTime() - lastUpdate);
	SetTimeToAct(sleep);

	//game updated, ask for a redraw
	Redraw(true);

}

void Bxf03uMain::DrawObjects()
{
	//draw stage
	stage.draw();
	//draw state information on top of it
	stateDisplay.DrawTileAt(this, m_pActualScreen, 0, state, 0, 0);
}


void Bxf03uMain::DrawScreenRectangleFast(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour) const
{
	//copied from BaseEngine
	if (iX2 < iX1) { int t = iX1; iX1 = iX2; iX2 = t; }
	if (iY2 < iY1) { int t = iY1; iY1 = iY2; iY2 = t; }

	//create new rect
	auto rect = SDL_Rect();

	//set coordinates
	rect.x = iX1;
	rect.y = iY1;
	rect.w = iX2 - iX1;
	rect.h = iY2 - iY1;

	//fast SDL drawing
	SDL_FillRect(m_pActualScreen, &rect, uiColour);
}
