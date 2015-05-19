#include "header.h"
#include <iostream>
#include "JPGImage.h"
#include "templates.h"
#include "BaseEngine.h"
#include "Bxf03uWall.h"
#include "Bxf03uHook.h"
#include "Bxf03uStage.h"
#include "BXf03uPlayer.h"
#include "Bxf03uObstacle.h"

Bxf03uStage::Bxf03uStage()
{
	//load background to memory
	bg = ImageData();
	bg.LoadImage("bg.png");
}


Bxf03uStage::~Bxf03uStage()
{
}


void Bxf03uStage::init(Bxf03uMain* engine)
{
	//save a pointer to the engine
	this->engine = engine;

	//destroy old objects so we can begin
	engine->DestroyOldObjects();
	engine->alphaStrokes.clear();

	//initialise offsets
	Xoffset = 0;
	Yoffset = 0;
	
	//and score
	score = 0;

	//initialise background position
	bgX = -150;
	bgY = -1600;

	//create walls and store them
	auto wallLeft = new Bxf03uWall(engine, this, true);
	auto wallRight = new Bxf03uWall(engine, this, false);
	//all stores are made with the added auto placement (-1 push_back()) logic in BaseEngine
	engine->StoreObjectInArray(-1, wallLeft);
	engine->StoreObjectInArray(-1, wallRight);

	//allocate hook and store
	//only allocate so that the player can receive the hook
	//and the hook can receive the player pointer
	hook = (Bxf03uHook*)malloc(sizeof(Bxf03uHook));
	engine->StoreObjectInArray(-1, hook);

	//create player and store
	player = new Bxf03uPlayer(engine, this, hook);
	engine->StoreObjectInArray(-1, player);

	//now we can create the hook since we have the player
	new (hook) Bxf03uHook(engine, this, player);

	//create obstacles form this location
	lastObstacleY = 600;

	//create 10 for starters
	for (int i = 0; i < 10; i++)
	{
		GenerateNewObstacle();
	}

	//save an iterator for the first one
	//for somewhat faster looping in update()
	firstObstacle = engine->DisplayableObjects.begin() + 4;

	godMode = false;
}

//taking over the update loop as the DisplayableObjects queue
//doesn't have the right update order
void Bxf03uStage::update(int time)
{
	//first update the player's stage location
	player->DoUpdate(time);

	//based on that update the stage offset
	float diff = Yoffset;
	Yoffset = -player->stageY + 600;
	Xoffset = -(player->stageX - 250)*0.7;
	diff -= Yoffset;

	//then update the player's screen location
 	player->m_iCurrentScreenX = toActualX(player->stageX);

	//and the background
	float parallax = 0.2;
	bgX = toStageX(-150 / parallax) * parallax;
	bgY += diff*(1-parallax);

	//tiling the background infinitely
	if (0 < toActualY(bgY)) {
		bgY -= 1600;
	}
	else if (toActualY(bgY) < -1600)
	{
		bgY += 1600;
	}

	//if the last obstacle is about to come on-screen create a new one
	if (-100 < toActualY(lastObstacleY)) {
		GenerateNewObstacle();
	}

	//now update the obstacles
	for (auto ob = firstObstacle; ob != engine->DisplayableObjects.end(); ob++)
	{
		if (*ob == NULL) break;
		
		(*ob)->DoUpdate(time);
	}

	//now that the obstacles are updated, try hooking again if needed
 	if (player->waitForHook) { player->HookOn(player->hookCentre); }

	//hooking is done update the hook
	hook->DoUpdate(time);

	//everything done check if the game is over
	bool insideWalls = 50 + player->radius <= player->stageX && player->stageX <= 450 - player->radius;
	if (!insideWalls && !(player->hooked)) over();

	//konami code
	int len = engine->alphaStrokes.size();
	if (10 <= len && engine->alphaStrokes.substr(len-10, std::string::npos) == "wwssadadqe")
	{
		godMode = true;
	}

	//save the new score
	score = toActualY(1050);
}

//taking over most of the draw loop as well
void Bxf03uStage::draw()
{
	//render the parallax background
	bg.RenderImage(engine->GetBackground(), 0, 0, toActualX(bgX), toActualY(bgY), 800, 2400);
	
	//copied from BaseEngine
	engine->m_iDrawableObjectsChanged = 0;
	// And this re-draws the new positions
	if (&(engine->DisplayableObjects) != NULL)
	{
		for each (DisplayableObject* ob in engine->DisplayableObjects)
		{
			if (ob == NULL) break;
			ob->Draw();
			if (engine->m_iDrawableObjectsChanged)
				return; // Abort! Something changed in the array
		}
	}

	if (engine->state != Bxf03uMain::running) return;

	//draw the score over everything
	char scoreString[29];
	sprintf(scoreString, "%28d", score);
	engine->DrawScreenString(20, 10, scoreString, 0xdddddd, engine->GetFont("consola.ttf", 30));
}

//stop the game
void Bxf03uStage::over()
{
	if (godMode) return;
	engine->state = Bxf03uMain::over;
	//state change, ask for a redraw
	engine->Redraw(true);
}



//takes care of generating new obstacles
void Bxf03uStage::GenerateNewObstacle()
{
	//a new obstacle every 150 pixels with some variation
	int locY = lastObstacleY - normalRandBetween(100, 200, 18);

	//make the average size of obstacles grow with the score
	int max = 10 + abs(Yoffset / 350);
	int size = normalRandBetween<int>(10, max, 10);
	if (size < 5) size = 5; //minimum size is 5
	
	//place the obstacle inside the walls
	int locX = realRandBetween(50 + size, 450 - size);

	//but don't place it in front of the player for the first few seconds
	bool inFrontOfPlayer = 240 < locX + size && locX - size < 260;
	if (-1000 < locY && inFrontOfPlayer)
	{
		while (inFrontOfPlayer)
		{
			locX = realRandBetween(50 + size, 450 - size);
			inFrontOfPlayer = 240 < locX + size && locX - size < 260;
		}
	}

	//but after a while make sure the player has to do something
	if (engine->DisplayableObjects.size() == 14)
	{
		//the 10th obstacle will be in the way
		locX = 250;
	}

	//placement done, create and store the obstacle
	auto obstacle = new Bxf03uObstacle(engine, this, locX, locY, 2*size);
	engine->StoreObjectInArray(-1, obstacle);

	//save its y location so we can generate new ones
	lastObstacleY = locY;

}
