#include <cmath>
#include "header.h"
#include <iostream>
#include "templates.h"
#include "Bxf03uStage.h"
#include "BXf03uPlayer.h"
#include "Bxf03uObstacle.h"

#define PI 3.14159265358979323846


Bxf03uPlayer::Bxf03uPlayer(BaseEngine* engine, Bxf03uStage* stage, Bxf03uHook* hook)
	: DisplayableObject(engine)
	, stage(stage)
	, hook(hook)
{
	//initialise screen and stage location, direction and speed
	m_iCurrentScreenY = 600; //never updated
	stageX = 250;
	stageY = 700;

	speed = 0.35;
	direction = 180; //upwards

	radius = 10;

	//initialise some other values too
	hooked = false;
	waitForHook = false;

	lastUpdate = engine->GetTime();
}


Bxf03uPlayer::~Bxf03uPlayer()
{
}

void Bxf03uPlayer::DoUpdate(int iCurrentTime)
{
	//calculate time since last update
	float diff = iCurrentTime - lastUpdate;
	if (40 < diff) {
		//////////////////////////////////////////////////////////////////////////
		//SDL skips a beat on a constant frequency and it messes up my calculations
		//here we are just ignoring those skips
		lastUpdate = iCurrentTime;
		//added benefit is that now I don't have to set a time offset when the game
		//is not running since it just gets ignored here
		return;
	}
	
	//making the player have a framerate independent speed
	float perTickVelocity = speed * diff;
	float perTickAngularVelocity = angularVelocity * diff;

	//save this update time
	lastUpdate = iCurrentTime;

	//add to the direction if we are hooked
	//essentially making the player move on a circular path
	direction += hooked ? perTickAngularVelocity : 0;

	if (360 < direction) {
		direction -= 360;
	}

	if (direction < 0) {
		direction += 360;
	}

	//calculate coordinate differences
	float dx = std::sin(direction*PI / 180) * perTickVelocity;
	float dy = std::cos(direction*PI / 180) * perTickVelocity;

	//update coordinates
	stageX += dx;
	stageY += dy;

}

void Bxf03uPlayer::Draw()
{

	//draw an oval using screen coordinates as center
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX - radius,
		m_iCurrentScreenY - radius,
		m_iCurrentScreenX + radius,
		m_iCurrentScreenY + radius,
		0xffffff);


}

//this makes the player try to hook on an obstacle
void Bxf03uPlayer::HookOn(Bxf03uObstacle* hookCentre) {
	//save a pointer to the obstacle
	this->hookCentre = hookCentre;

	//and store it's coordinates
	hookX = hookCentre->stageX;
	hookY = hookCentre->stageY;

	//calculate the visible angle
	//ie if the player is heading towards the obstacle angleFromFront will be 0
	//if it is on the left or right it's going to be between 0 and 180
	float x1 = std::sin(direction * PI / 180);
	float y1 = std::cos(direction * PI / 180);
	float x2 = hookX - this->stageX;
	float y2 = hookY - this->stageY;
	float angleFromFront = GetVectorAngle(x1, y1, x2, y2);
	bool hookBehind = 90 < angleFromFront;

	//if it's in front of the player then wait
	waitForHook = !hookBehind;
	if (waitForHook) return;

	//otherwise hook on
	hooked = true;
	//tell Bxf03uHook
	hook->HookOn(hookCentre);
	//and Bxf03uObstacle
	hookCentre->HookOn();

	//calculate the circular trajectory's radius
	float radius = sqrt(pow(stageX - hookX, 2) + pow(stageY - hookY, 2));

	//and the speed needed to have constant velocity
	float oneTurn = 2 * radius * PI / speed;
	angularVelocity = 360/oneTurn;
	
	//decide what side the obstacle is on
	x1 = std::sin((direction + 90)*PI / 180);
	y1 = std::cos((direction + 90)*PI / 180);
	float angleFromLeft = GetVectorAngle(x1, y1, x2, y2);
	bool hookToTheRight = 90 < angleFromLeft;

	//if it's on the right then reverse the angular velocity
	//that is move in a clockwise direction
	if (hookToTheRight) {
		//right cw
		angularVelocity = -angularVelocity;
		angleFromLeft -= 180;
	}

	//turn the player so that it is in the right angle
	//wrt the obstacle
	direction += hookBehind ? angleFromLeft : -angleFromLeft;

}

//release hook
void Bxf03uPlayer::HookOff()
{
	hooked = false;
	waitForHook = false;
	//tell the hook as well
	hook->HookOff();
}


//utility function to get the relative angle between two vectors
float Bxf03uPlayer::GetVectorAngle(float x1, float y1, float x2, float y2) const {
	float top = (x1 * x2) + (y1 * y2);
	float bot = sqrt((x1*x1) + (y1*y1)) * sqrt((x2*x2) + (y2*y2));
	float rad = std::acos(top / bot);

	float ang = (180 / PI) * rad;

	return ang;
}
