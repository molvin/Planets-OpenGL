#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace utils
{
	inline std::string ReadFile(const std::string& path)
	{
		printf("Reading file: %s \n", path.c_str());
		std::stringstream fileStream;
		std::string line;
		std::ifstream file(path);
		if(!file.is_open())
		{
			//TODO: log
			printf("error reading file");
			return "";
		}
		while(std::getline(file, line))
		{
			fileStream << line << "\n";
		}
		file.close();
		return fileStream.str();		
	}
}

