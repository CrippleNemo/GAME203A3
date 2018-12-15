#include <iostream>
#include <fstream>
#include <filesystem>
#include "FileHandler.h"
#include "GameEditor.h"

using namespace std;
namespace fs = std::experimental::filesystem::v1;
int main(int argc, char* args[])
{
	GameEditor Editor;
	FileHandler Handler;
	string fileName;

	//Listing all json files in the levels directory
	cout << "List of level files:" << endl;
	Handler.DisplayDirectory("Levels");

	cout << "\nType in the name of your level choose a file to load. Please make sure to type file type aswell." << endl;
	cin >> fileName;

	fileName = Handler.CheckExtension(fileName,".json");

	//Checks if the file exists.
	if (Handler.FileCheck("Levels\\" + fileName))
	{
		//Makes json file in levels directory.
		Handler.CreateFile("Levels\\" + fileName);
		cout << "\nFile " + fileName + " has been created." << endl;

		while (!Editor.quit)
		{
			cout << "Choose your background picture: " << endl;
			Handler.DisplayDirectory("Assets\\Backgrounds");

			string bgName;
			cin >> bgName;

			//Checking to see if the background image exists
			if (Handler.FileCheck("Assets\\Backgrounds\\" + bgName))
				cout << "Background does not exists. Try again." << endl;			
			else
				cout << "Successfully Loaded: " << fileName << " with " << bgName << endl;
				Editor = GameEditor(fileName, bgName);
		}
	}
	else
	{
		cout << "Successfully Loaded: " << fileName << endl;
		Editor = GameEditor(fileName);
	}

	getchar();
	return 0;
}