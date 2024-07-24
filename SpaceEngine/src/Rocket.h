#pragma once

#include "Vec3Double.h"

class Rocket
{
public:
	double mass;
	Vec3Double position;
	
	Vec2 thrustVector;

public:
	Rocket() = default;
	~Rocket() = default;
};