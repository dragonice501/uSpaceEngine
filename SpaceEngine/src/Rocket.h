#pragma once

#include "CelestialBody.h"
#include "Vec3Double.h"

class Rocket
{
public:
	double mass;
	Vec3Double position;
	
	Vec3Double forcesVector;
	Vec2 thrustVector;
	Vec3Double acceleration;
	Vec3Double velocity;

	float gravity;
	int alttitude;
	int radius;

public:
	Rocket() = default;
	~Rocket() = default;

	void Update(const float dt, const CelestialBody& centralBody);
};