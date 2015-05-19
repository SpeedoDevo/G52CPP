#pragma once
#include <set>
#include <string>
#include <fstream>
#include <utility>
#include "TileManager.h"
#include "Bxf03uStage.h"

class Bxf03uTileManager :
	public TileManager
{
public:
	Bxf03uTileManager(int width, int height, Bxf03uStage* stage);
	~Bxf03uTileManager();
	void InitFonts();
	virtual void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const;

	void reset() const;

private:
	unsigned int color;
	SDL_Surface* backdrop;
	
	mutable std::fstream file;

	mutable bool written;
	mutable std::string name;
	mutable std::set< std::pair<int, std::string> > scores;
	
	mutable bool allBeaten;
	mutable int toBeatCounter;
	mutable char toBeatString[128];
	mutable std::set< std::pair<int, std::string> >::iterator toBeat;
	

	Font* bigFont;
	Font* mediumFont;
	Font* smallFont;
	Font* titleFont;
	Font* bigMediumFont;
	
	Bxf03uStage* stage;

	bool WriteScores() const;
	bool ReadScores();
	bool SaveIfHighscore(int score) const;
};

