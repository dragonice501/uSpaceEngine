#pragma once

#include "Scene.h"

#include "CelestialBody.h"

class OrbitScene : public Scene
{
private:
	CelestialBody sun;

	CelestialBody mercury;
	CelestialBody venus;
	CelestialBody earth;
	CelestialBody mars;
	CelestialBody jupiter;
	CelestialBody saturn;
	CelestialBody uranus;
	CelestialBody neptune;
	CelestialBody pluto;

	std::vector<CelestialBody*> planets;

	double systemSize = 170e11;

public:
	OrbitScene() {}

	void Setup() override;
	void Destroy() override;
	void Input() override;
	void Update(float deltaTime) override;
	void Render() override;

	void CalculatePosition(CelestialBody& planet);

	void DrawOrbit(const CelestialBody& planet);
};