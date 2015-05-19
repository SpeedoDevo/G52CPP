#include <string>
#include <iostream>
#include "header.h"
#include "Bxf03uMain.h"
#include "Bxf03uStage.h"
#include "Bxf03uObstacle.h"
#include "Bxf03uTileManager.h"


Bxf03uTileManager::Bxf03uTileManager(int width, int height, Bxf03uStage* stage)
	: TileManager(width,height)
	, stage(stage)
{
	//set size and values
	SetSize(1, 4);
	SetValue(0, Bxf03uMain::started, Bxf03uMain::started);
	SetValue(0, Bxf03uMain::over, Bxf03uMain::over);
	SetValue(0, Bxf03uMain::highscores, Bxf03uMain::highscores);
	SetValue(0, Bxf03uMain::running, Bxf03uMain::running);

	//base position is 0,0
	SetBaseTilesPositionOnScreen(0, 0);

	//try to open the name file
	file.open("name.hookr", std::fstream::in);
	if (file.is_open()) {
		//found read it
		getline(file, name);
	}
	else
	{
		//not found, try to write it
		file.close();
		file.open("name.hookr", std::fstream::in | std::fstream::out | std::fstream::trunc);
		if (file.is_open()) {
			//write file with default name
			file << "hookr";
			file.flush();
			//set default name
			name = "hookr";
		}
		else
		{
			//couldn't write file
			printf("\nset your name manually in name.hookr\n\n");
			name = "hookr";
		}
	}
	file.close();

	//try to open the scores file
	file.open("scores.hookr", std::fstream::in);
	if (file.is_open()) {
		//found, read scores
		if (ReadScores())
		{
			//error
			printf("Couldn't open scores file.\n");
		}
	}
	else
	{
		//not found, generate default highscores
		
		//set of pairs used as it's automatically sorted
		//and it can have the same score and name multiple times
		scores.insert(std::pair<int, std::string>(1000, "larry"));
		scores.insert(std::pair<int, std::string>(2000, "abrar"));
		scores.insert(std::pair<int, std::string>(3000, "dijkstra"));
		scores.insert(std::pair<int, std::string>(4000, "heisenberg"));
		scores.insert(std::pair<int, std::string>(5000, "prof. moriarty"));
		scores.insert(std::pair<int, std::string>(6000, "turing"));
		scores.insert(std::pair<int, std::string>(7000, "hawking"));
		scores.insert(std::pair<int, std::string>(8000, "speedodevo"));
		scores.insert(std::pair<int, std::string>(9000, "deadly kitten"));
		scores.insert(std::pair<int, std::string>(10000, "slim shady"));
		
		//write them
		if (WriteScores())
		{
			//error
			printf("Couldn't open scores file.\n");
		}
	}
	//keep this file open

	//the one to beat will be the lowest scorer
	toBeat = scores.begin();
	//for making the achievement disappear
	toBeatCounter = 1000;
	
	//default font color
	color = 0xdddddd;

	//transparency is not supported by the BaseEngine, SDL workaround
	//initialise a semi transparent black background to make the text more readable
	backdrop = SDL_CreateRGBSurface(0, 500, 700, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL_FillRect(backdrop, NULL, SDL_MapRGBA(backdrop->format, 0, 0, 0, 75));
}

void Bxf03uTileManager::InitFonts()
{
	//initialise some fonts
	titleFont = stage->engine->GetFont("consola.ttf", 146);
	bigFont = stage->engine->GetFont("consola.ttf", 50);
	bigMediumFont = stage->engine->GetFont("consola.ttf", 36);
	mediumFont = stage->engine->GetFont("consola.ttf", 24);
	smallFont = stage->engine->GetFont("consola.ttf", 16);
}

Bxf03uTileManager::~Bxf03uTileManager()
{
	//close the scores file
	file.close();
}

void Bxf03uTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const {
	//backdrop needed in all states except when running, draw that first
	if (iMapY != Bxf03uMain::running) SDL_BlitSurface(backdrop, NULL, pEngine->GetForeground(), NULL);

	//which tile to draw
	switch (iMapY)
	{
	case Bxf03uMain::started:
		//draw welcome
		pEngine->DrawScreenString(50, 20, "hookr", color, titleFont);
		pEngine->DrawScreenString(50, 500, "press space to start", color, bigMediumFont);

		//instructions
		pEngine->DrawScreenString(50, 630, "press esc any time to quit", color, smallFont);
		pEngine->DrawScreenString(50, 650, "press n to show the highscores", color, smallFont);
		pEngine->DrawScreenString(50, 670, "press m any time to restart", color, smallFont);
		break;

	case Bxf03uMain::over:
		{
		pEngine->DrawScreenString(50, 50, "game over", color, bigFont);
		pEngine->DrawScreenString(50, 100, "your score was:", color, bigFont);

		//get the score
		int score = stage->score;
		//save it maybe
		bool isHighscore = SaveIfHighscore(score);
		//draw it
		char scoreString[128];
		sprintf(scoreString, "%d", score);
		pEngine->DrawScreenString(50, 150, scoreString, color, bigFont);

		//notify if it made it to highscores
		//checks for written as isHighscore is false if the score is equal
		if (isHighscore || written)
		{
			pEngine->DrawScreenString(50, 325, "new highscore!", color, bigFont);
		}

		//reset achievements
		reset();

		//draw instructions
		pEngine->DrawScreenString(50, 650, "press n to show the highscores", color, smallFont);
		pEngine->DrawScreenString(50, 670, "press m to restart", color, smallFont);
		}
		break;
	case Bxf03uMain::highscores:
		{
		//title
		pEngine->DrawScreenString(50, 50, "highscores", color, bigFont);

		//loop over scores in reverse order
		int count = 1;
		std::set<std::pair<int, std::string>>::reverse_iterator it;
		for (it = scores.rbegin(); it != scores.rend(); it++)
		{
			char scoreString[128];
			//create uniform table
			sprintf(scoreString, "%2d: %5d by %s", count, it->first, it->second.c_str());

			//green if it is your score
			unsigned int scoreColor = it->second == name ? 0x83f52c : color;
			//purple if it was the highscore of the last game
			scoreColor = stage->score == it->first ? 0x9b59b6 : scoreColor;

			//draw it
			pEngine->DrawScreenString(50, 200 + (++count * 25), scoreString, scoreColor, mediumFont);
		}

		//notify that you still have the default name
		if (name == "hookr") pEngine->DrawScreenString(50, 500, "you can set your name in name.hookr", color, smallFont);

		//draw instructions
		pEngine->DrawScreenString(50, 670, "press m to restart", color, smallFont);
		}
		break;
	case Bxf03uMain::running:
		//if the lowest score was just beaten
		if (!allBeaten && toBeat->first < stage->score)
		{
			//check if you've beat yourself
			std::string maybeYourScore = toBeat->second == name ? "your own highscore" : toBeat->second;
			sprintf(toBeatString, "you've just beaten %s", maybeYourScore.c_str());
			
			//set the next target to the next lowest score
			toBeat++;
			
			//if everyone was beaten don't check any more
			allBeaten = toBeat == scores.end();
			toBeatCounter = 0;
		}
		
		//hide after a while
		if (++toBeatCounter < 1000)
		{
			pEngine->DrawScreenString(20, 10, toBeatString, color, smallFont);
		}

		//state change, reset written
		written = false;
		break;
	}

}


bool Bxf03uTileManager::ReadScores()
{
	//double checking
	if (!file) return true;
	std::string line;
	//read in lines one by one
	while (getline(file, line))
	{
		//rightmost space is the separator
		auto pos = line.rfind(' ');
		//name is before that
		std::string name = line.substr(0, pos);
		try
		{
			//score is after that
			int score = stoi(line.substr(pos, std::string::npos));
			//create the entry
			auto p = std::pair<int, std::string>(score, name);
			scores.insert(p);
			//keep scores to a size of 10
			if (10 < scores.size()) scores.erase(scores.begin());
		}
		catch (...)
		{
			//catch if the int couldn't be parsed (stoi)
			printf("Score couldn't be read\n");
		}
	}
	//success
	return false;

}

bool Bxf03uTileManager::WriteScores() const
{
	file.close();
	//open the scores file (clear it)
	file.open("scores.hookr", std::fstream::in | std::fstream::out | std::fstream::trunc);
	//error
	if (!file) return true;
	//write each score with the specified format
	for each (auto score in scores)
	{
		file << score.second << " " << score.first << std::endl;
	}
	//flush the file
	file.flush();
	//success
	return false;
}

bool Bxf03uTileManager::SaveIfHighscore(int score) const
{
	//if it is a highscore
	bool isHighscore = scores.begin()->first < score || scores.size() < 10;
	//and it hasn't been written to the file
	if (isHighscore && !written) {
		//insert it
		scores.insert(std::pair<int, std::string>(score, name));
		//and remove the smallest if necessary
		if (10 < scores.size()) scores.erase(scores.begin());

		//write it to the scores file
		WriteScores();
		//don't write it repeatedly
		written = true;
	}

	//return if it was a highscore
	return isHighscore;
}

void Bxf03uTileManager::reset() const
{
	//reset toBeat
	toBeat = scores.begin();
	toBeatCounter = 1000;
	allBeaten = false;
}
