#pragma once

#include "Vec2.h"
#include "Vec3Double.h"
#include "PhysicsConstants.h"

#include <stdint.h>

class CelestialBody
{
public:
	// Physical Properties
	double mass;
	int massExponent;
	double radius;
	Vec3Double position;
	Vec3Double velocity;

	// Kepler Orbit Elements
	double semiMajorAxis;
	int semiMajorExponent;
	float eccentricity;
	float inclination = 0.0f;
	float ascLongitude = 0.0f;
	float argPeriapsis = 0.0f;
	float trueAnomaly = 0.0f;

	// Helper Elements
	float meanAnomaly = 0.0f;
	double n = 0.0;
	double period = 0.0f;
	double currentPeriod = 0.0f;

	// Render Properties
	int size;
	uint32_t color;
	Vec3Double orbitPoints[32];


public:
	CelestialBody() = default;
	~CelestialBody() = default;

	void CalculateParametersAndElements(const CelestialBody& M);

	void CalculateOrbitPoints(const double systemSize);
};