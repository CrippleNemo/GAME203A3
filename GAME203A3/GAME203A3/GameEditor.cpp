#include <iostream>
#include <fstream>
#include <string>
#include <SDL_image.h>

#include "GameEditor.h"

GameEditor::GameEditor(){}

GameEditor::GameEditor(std::string filename_, std::string bgimg_)
{
	bgimg = bgimg_;
	fileName = filename_;

	Initialize();
}

GameEditor::GameEditor(std::string fileName_)
{
	//JSON loading
	fileName = fileName_;
	std::ifstream fileIn("Levels\\" + fileName);
	fileIn >> jsonObjects;

	//Starting at index 1 to exclude background
	for (auto i = 1; i < jsonObjects.size(); i++)
	{
		SDL_Rect tempRect;
		tempRect.h = jsonObjects[i][1][0];
		tempRect.w = jsonObjects[i][1][1];
		tempRect.x = jsonObjects[i][1][2];
		tempRect.y = jsonObjects[i][1][3];

		//inserting objects from json object into multimap.
		objects.insert(std::pair<std::string, SDL_Rect>(jsonObjects[i][0], tempRect));
	}

	bgimg = jsonObjects[0][0].get<std::string>();

	Initialize();
}

GameEditor::~GameEditor() {}

void GameEditor::Initialize()
{
	//Printing a hotkey legend to the console
	std::cout << "\n###Controls###" << std::endl;
	std::cout << "# S: Save" << std::endl;
	std::cout << "# Q: Quit " << std::endl;
	std::cout << "# Number keys: Objects\n" << std::endl;
	std::cout << ("Current asset selected: " + currentObject) << std::endl;

	//Initializing background info
	bgSurface = IMG_Load(("Assets\\Backgrounds\\" + bgimg).c_str());
	bgRect.h = bgSurface->h;
	bgRect.w = bgSurface->w;
	bgRect.x = 0;
	bgRect.y = 0;

	//Initializing currentObject
	currentObject = "obj_FoodBowlEmpty.png";

	//Initializing the SDL Window
	const int WINDOW_WIDTH = bgRect.w;
	const int WINDOW_HEIGHT = bgRect.h;

	window = SDL_CreateWindow("Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	screenSurface = SDL_GetWindowSurface(window);

	//Adding the background to the map as the first entry
	AddObject(bgimg, 0, 0); 
	RenderObjects();

	EventHandler();
}

void GameEditor::EventHandler()
{
	SDL_Event ev;
	SDL_PumpEvents();
	
	//while loop to handle SDL Events.
	while (!quit)
	{
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&xMousePos, &yMousePos);
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					AddObject(currentObject, xMousePos, yMousePos);
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT)
				{
					Remove(xMousePos, yMousePos);
				}
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym) {
				case SDLK_s:
					SaveFile();
					break;

				case SDLK_q:
					SDL_DestroyWindow(window);
					quit = !quit;
					break;

				//Object selection cases
				case SDLK_1:
					currentObject = ("obj_FoodBowlEmpty.png");
					std::cout << ("Current asset selected: " + currentObject) << std::endl;
					break;
				case SDLK_2:
					currentObject = ("obj_couch.png");
					std::cout << ("Current asset selected: " + currentObject) << std::endl;
					break;
				case SDLK_3:
					currentObject = ("obj_dresser.png");
					std::cout << ("Current asset selected: " + currentObject) << std::endl;
					break;
				case SDLK_4:
					currentObject = ("obj_LitterEmpty.png");
					std::cout << ("Current asset selected: " + currentObject) << std::endl;
					break;

				default:
					break;
				}
			}
		}
	}
}

void GameEditor::AddObject(std::string imageName, int xMousePos, int yMousePos)
{
	SDL_Surface* objImage;
	SDL_Rect imgRect;

	//Loading images and storing it into the map. With background image first.
	if (imageName != bgimg)
	{
		objImage = IMG_Load(("Assets\\Objects\\" + imageName).c_str());
		imgRect.h = objImage->h;
		imgRect.w = objImage->w;
		imgRect.x = xMousePos - (imgRect.w / 2);
		imgRect.y = yMousePos - (imgRect.h / 2);
		objects.insert(std::pair<std::string, SDL_Rect>(imageName, imgRect));
	}
	else
	{
		objects.insert(std::pair<std::string, SDL_Rect>(bgimg, bgRect));
	}

	RenderObjects();
}

//Removes from the map by mouse position. 
void GameEditor::Remove(int xMousePos, int yMousePos)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (xMousePos > it->second.x && xMousePos < (it->second.x + it->second.w))
		{
			if (yMousePos > it->second.y && yMousePos < (it->second.y + it->second.h))
			{
				if (it->first != bgimg)
				{
					objects.erase(it);
					it = objects.begin();
				}
			}
		}
	}

	RenderObjects();
}

//Rendering all objects to the screen.
void GameEditor::RenderObjects()
{
	SDL_Surface* bgSurface = IMG_Load(("Assets\\Backgrounds\\" + bgimg).c_str());
	SDL_BlitSurface(bgSurface, nullptr, screenSurface, &bgRect);

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		SDL_Surface* objImage = IMG_Load(("Assets\\Objects\\" + it->first).c_str());
		SDL_BlitSurface(objImage, nullptr, screenSurface, &it->second);
	}

	SDL_UpdateWindowSurface(window);
}

//Saving the level by putting the objects into a json file. 
void GameEditor::SaveFile()
{	
	//only can save level if both of these images are in the level.
	if (objects.find("obj_FoodBowlEmpty.png") != objects.end() && objects.find("obj_LitterEmpty.png") != objects.end())
	{
		std::cout << "Successfully wrote file to " + fileName << std::endl;
		
		jsonObjects.clear();
		
		//[imagename,[h,w,x,y]]
		int counter = 0;
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			jsonObjects[counter] = {it->first,{it->second.h,it->second.w,it->second.x,it->second.y}};
			counter++;
		}

		std::ofstream newFile;
		newFile.open("Levels\\" + fileName);
		newFile << jsonObjects.dump(4);
		newFile.close();
	}
	else
	{
		std::cout << "Could not write file. Please add a food bowl and/or litter box to the level." << std::endl;
	}
}