#pragma once
#include "Planet.h"
#include "../Graphics/Light/DirectionalLight.h"
#include "../Graphics/Light/AmbientLight.h"

class Window;
class Camera;

class SolarSystem
{
public:
	SolarSystem();

	void Update(Camera& camera, Window& window);
	void Render(const glm::vec3& eyePosition);
	void DrawGui();
	void SetShadowBuffer(const Texture* shadowBuffer);
	DirectionalLight& GetGodLight() { return _godLight; }
	
private:
	void CreatePlanet();
	void Save();
	void Load();
	bool CheckIntersect(glm::vec3 origin, glm::vec3 rayDir, glm::vec3 min, glm::vec3 max, glm::mat4 model, float& intersectDistance);
	void ScreenPosToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec3& out_origin, glm::vec3& out_direction);
	
	std::vector<Planet> _planets;
	std::string _path = "res/SolarSystem";
	int _planetCounter = 0;

	Planet* _selectedPlanet = nullptr;
	DirectionalLight _godLight;
	AmbientLight _ambientLight;
	const Texture* _shadowBuffer = nullptr;
	bool _hideUi;
};

