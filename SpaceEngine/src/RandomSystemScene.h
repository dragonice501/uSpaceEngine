#pragma once

#include "Scene.h"

#include "CelestialBody.h"

class RandomSystemScene : public Scene
{
private:
	CelestialBody sun;

	double systemMax = 10e11;

	float sunSize;
	int sunExponent;

public:
	RandomSystemScene() {}

	void Setup() override;
	void Destroy() override;
	void Input() override;
	void Update(float deltaTime) override;
	void Render() override;

	void GenerateSun();
};