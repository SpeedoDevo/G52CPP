#include "Bxf03uHook.h"


Bxf03uHook::Bxf03uHook(BaseEngine* engine, Bxf03uStage* stage, Bxf03uPlayer* player)
	: DisplayableObject(engine)
	, player(player)
	, stage(stage)
{
	//initially not visible
	m_bVisible = false;
}


Bxf03uHook::~Bxf03uHook()
{
}

void Bxf03uHook::Draw()
{
	//don't draw if not visible
	if (!m_bVisible) return;

	//draw a line from the player to the obstacle
	GetEngine()->DrawScreenLine(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX2, m_iCurrentScreenY2,
		0xfff500);

	//draw a 2px hollow circle, as the trajectory for the player
	GetEngine()->DrawHollowOval(
		m_iCurrentScreenX2 - radius - 1, m_iCurrentScreenY2 - radius - 1,
		m_iCurrentScreenX2 + radius + 1, m_iCurrentScreenY2 + radius + 1,
		m_iCurrentScreenX2 - radius + 1, m_iCurrentScreenY2 - radius + 1,
		m_iCurrentScreenX2 + radius - 1, m_iCurrentScreenY2 + radius - 1,
		0xfff500, GetEngine()->GetForeground());


}

void Bxf03uHook::DoUpdate(int iCurrentTime)
{
	// fetch screen locations from the objects involved
	if (!m_bVisible) return;
	m_iCurrentScreenX = player->m_iCurrentScreenX;
	m_iCurrentScreenY = player->m_iCurrentScreenY;
	m_iCurrentScreenX2 = obstacle->m_iCurrentScreenX;
	m_iCurrentScreenY2 = obstacle->m_iCurrentScreenY;
}


void Bxf03uHook::HookOn(Bxf03uObstacle* obstacle)
{
	//set the obstacle
	this->obstacle = obstacle;
	//set visible
	m_bVisible = true;

	//calculate the radius
	float a = player->stageX - obstacle->stageX;
	float b = player->stageY - obstacle->stageY;
	radius = sqrt(a*a + b*b);

}

//make invisible
void Bxf03uHook::HookOff()
{
	m_bVisible = false;
	obstacle = nullptr;
}
