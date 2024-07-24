#pragma once

#include "Scene.h"

#include "CelestialBody.h"

#include <vector>

class SystemCreatorScene : public Scene
{
private:
	CelestialBody sun;
	CelestialBody* selectedPlanet;

	std::vector<CelestialBody> planets;

	Vec3Double systemOffset;

	double systemSizeDefault = 20e11;
	double systemSize = 20e11;

	float sunMass = 1.989f;

	float mass = 3.25f;
	int massExponent = 23;

	float size = 10;
	float semiMajor = 5.793684f;
	int semiMajorExponent = 10;
	float eccentricity = 0.205f;
	float argPeriapsis = 0.0f;
	float trueAnomaly = 0.0f;
	float color[4];
	uint32_t trueColor = 0xffffffff;

	bool simulateOrbit = false;

	bool mouseButtonDown;
	bool click = false;
	double maxSelectionDistance = 10e9;

	int guiWidth, guiHeight;

public:
	void Setup() override;
	void Destroy() override;
	void Input() override;
	void Update(float deltaTime) override;
	void Render() override;

	void DrawOrbit(const CelestialBody& planet);
	void DrawGUI();

	void SelectNearestPlanet(const Vec3Double& selectionPoint);
	void SetSunSettings(const float mass);
	void CalculatePlanetPosition(CelestialBody* planet);
	void CalculatePlanetOrbit(CelestialBody* planet);
};