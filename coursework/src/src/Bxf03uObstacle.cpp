#include "header.h"
#include "templates.h"
#include "Bxf03uStage.h"
#include "Bxf03uObstacle.h"


Bxf03uObstacle::Bxf03uObstacle(BaseEngine* engine, Bxf03uStage* stage, int stageX, int stageY, int diameter)
	: DisplayableObject(engine)
	, stageX(stageX)
	, stageY(stageY)
	, radius(diameter/2)
	, stage(stage)
{
	//choose a random color out of these 6
	switch (realRandBetween(0, 5))
	{
	case 0:
		color = 0x228dff;
		break;
	case 1:
		color = 0xfa023c;
		break;
	case 2:
		color = 0x0dd5fc;
		break;
	case 3:
		color = 0xff0099;
		break;
	case 4:
		color = 0x83f52c;
		break;
	case 5:
		color = 0xff6700;
		break;
	}

	//but save individual color values as well for the white fading
	r = (color & 0xff0000) >> 16;
	g = (color & 0x00ff00) >> 8;
	b = (color & 0x0000ff);

	//initially don't do fading
	transitionFinished = true;
}


Bxf03uObstacle::~Bxf03uObstacle()
{
}


void Bxf03uObstacle::Draw()
{
	//don't draw if off-screen
	if (!onScreen) return;

	GetEngine()->DrawScreenOval(
		x1,	y1,
		x2,	y2,
		color);
}

void Bxf03uObstacle::DoUpdate(int iCurrentTime)
{
	//transform stage coordinates
	m_iCurrentScreenX = stage->toActualX(stageX);
	m_iCurrentScreenY = stage->toActualY(stageY);

	//calculate coordinates for the circle
	x1 = m_iCurrentScreenX - radius;
	y1 = m_iCurrentScreenY - radius;
	x2 = m_iCurrentScreenX + radius;
	y2 = m_iCurrentScreenY + radius;
	
	//calculate if on-screen
	onScreen = 0 < x2 && 0 < y2 &&  x1 < GetEngine()->GetScreenWidth() && y1 < GetEngine()->GetScreenHeight();

	//transition color if necessary
	if (!transitionFinished) {
		//guarding to not exceed 255
		r = (r < 255) ? r + rSpeed : 255;
		g = (g < 255) ? g + gSpeed : 255;
		b = (b < 255) ? b + bSpeed : 255;
		
		//remix to uint
		color = ((int)r * 256 * 256) + ((int)g * 256) + (int)b;
		
		//finish if the color is white
		if (0xffffff <= color) {
			color = 0xffffff;
			transitionFinished = true;
		}
	}

	//calculate the distance to the player
	distanceToPlayer = FindDistanceTo(stage->player->stageX, stage->player->stageY);
	
	//if colliding then end game
	if (distanceToPlayer - radius - stage->player->radius <= 0)
	{
		stage->over();
	}
}

//just needed for transitioning to white
void Bxf03uObstacle::HookOn()
{
	if (!transitionFinished) return;
	//transition to white with an equal speed on all colors
	transitionFinished = false;
	rSpeed = (255 - r) / 100.f;
	gSpeed = (255 - g) / 100.f;
	bSpeed = (255 - b) / 100.f;
}

//do i need to explain?
float Bxf03uObstacle::FindDistanceTo(int x, int y)
{
	int a = x - stageX;
	int b = y - stageY;
	return sqrt(a*a + b*b);
}
