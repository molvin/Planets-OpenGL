#pragma once
#include "Planet.h"

class SolarSystem
{
public:
	SolarSystem();

	void Render();
	void DrawGui();

private:
	void CreatePlanet();
	void Save();
	void Load();
	
	std::vector<Planet> _planets;
	std::string _path = "res/SolarSystem";
	int _planetCounter = 0;
};

