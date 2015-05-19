#include "header.h"
#include "BaseEngine.h"
#include "Bxf03uWall.h"
#include "Bxf03uStage.h"
#include "DisplayableObject.h"


Bxf03uWall::Bxf03uWall(BaseEngine* engine, Bxf03uStage* stage, bool onLeft)
	: DisplayableObject(engine)
	, engine(dynamic_cast<Bxf03uMain*>(engine)) //we need DrawScreenRectangleFast
	, stage(stage)
	, onLeft(onLeft)
{}


Bxf03uWall::~Bxf03uWall()
{
}

void Bxf03uWall::Draw()
{
	if (onLeft)
	{
		//get screen position
		int x2 = stage->toActualX(50);
		if (x2 < 0) return; //don't draw if off-screen

		//draw black rectangle
		engine->DrawScreenRectangleFast(
			0, 0,
			x2, 700,
			0x000000);

		//blue if hooked red otherwise
		unsigned int color = stage->player->hooked ? 0x228dff : 0xfa023c;

		//draw wall markers
		engine->DrawScreenRectangleFast(
			x2 - 20, 0,
			x2 - 25, 700,
			color);

		engine->DrawScreenRectangleFast(
			x2 - 15, 0,
			x2, 700,
			color);
	}
	else
	{
		//get screen position
		int x1 = stage->toActualX(450);
		if (500 < x1) return;

		//draw black rectangle
		engine->DrawScreenRectangleFast(
			x1, 0,
			500, 700,
			0x000000);

		//blue if hooked red otherwise
		unsigned int color = stage->player->hooked ? 0x228dff : 0xfa023c;

		//draw wall markers
		engine->DrawScreenRectangleFast(
			x1,
			0,
			x1 + 15,
			700,
			color);

		engine->DrawScreenRectangleFast(
			x1 + 20,
			0,
			x1 + 25,
			700,
			color);
	}
}
