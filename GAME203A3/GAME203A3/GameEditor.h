#ifndef GameEditor_H
#define GameEditor_H

#include <SDL.h>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GameEditor
{
private:

	//Variables
	std::string fileName;
	std::string bgimg;

	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* bgSurface;
	SDL_Rect bgRect;

	int xMousePos, yMousePos;
	
	std::string currentObject;
	std::multimap<std::string, SDL_Rect> objects;
	json jsonObjects;

	//Methods
	void Initialize();
	void EventHandler();
	void AddObject(std::string imagename, int xMousePos, int yMousePos);
	void Remove(int xMousePos, int yMousePos);
	void RenderObjects();
	void SaveFile();

public:
	//Variables
	bool quit = false;

	//Methods
	GameEditor();
	GameEditor(std::string fileName_, std::string bgimg_);
	GameEditor(std::string fileName_);
	~GameEditor();
};

#endif