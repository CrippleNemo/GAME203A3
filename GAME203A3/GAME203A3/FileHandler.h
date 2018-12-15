#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <memory>
#include <fstream>
#include <filesystem>
#include "GameEditor.h"

namespace fs = std::experimental::filesystem::v1;

class FileHandler
{
public:
	FileHandler();
	~FileHandler();
	void DisplayDirectory(std::string directory);
	bool FileCheck(std::string filePath);
	void CreateFile(std::string fileName_);
	std::string CheckExtension(std::string pathName_, std::string ext);
};
#endif 