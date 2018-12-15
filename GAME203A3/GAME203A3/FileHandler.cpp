#include "FileHandler.h"

FileHandler::FileHandler(){}
FileHandler::~FileHandler(){}

void FileHandler::DisplayDirectory(std::string directory)
{
	for (auto& p : fs::directory_iterator(directory))
	{
		std::cout << p.path().filename() << std::endl;
	}
}
bool FileHandler::FileCheck(std::string filePath)
{
	bool fileNotFound;
	std::ifstream fileExists(filePath);
	if (fileExists)
	{
		fileNotFound = false;
	}
	else
	{
		fileNotFound = true;
	}
	return fileNotFound;
}

void FileHandler::CreateFile(std::string fileName_)
{
	std::ofstream outFile(fileName_);
}

std::string FileHandler::CheckExtension(std::string fileName_, std::string ext)
{
	if (fs::path(fileName_).extension().compare(ext))
	{
		fileName_ = fileName_.substr(0, fileName_.find_last_of('.')).append(ext);
		std::cout << "Your file extension has been changed to " + ext << std::endl;
	}
	return fileName_;
}