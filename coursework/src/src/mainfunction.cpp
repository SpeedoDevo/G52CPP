#include "header.h"

#include <ctime>

#include "templates.h"

// Needs one of the following #includes, to include the class definition
#include "BouncingBallMain.h"
#include "SimpleDemo.h"
#include "Demo1Main.h"
#include "Demo2Main.h"
#include "Demo3Main.h"
#include "Demo4Main.h"
#include "Bxf03uMain.h"

#define BASE_SCREEN_WIDTH 500
#define BASE_SCREEN_HEIGHT 700



int main(int argc, char *argv[])
{
	int iResult;

	// Send random number generator with current time
	::srand(time(0));

	Bxf03uMain oMain;

	iResult = oMain.Initialise("hookr", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "consola.ttf", 24);
	iResult = oMain.MainLoop();
	oMain.Deinitialise();

	return iResult;
}
