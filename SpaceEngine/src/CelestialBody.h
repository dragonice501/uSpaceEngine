#pragma once

#include "Vec2.h"
#include "Vec3Double.h"
#include "PhysicsConstants.h"

class CelestialBody
{
public:
	// Physical Properties
	double mass;
	double radius;
	Vec3Double position;
	Vec3Double velocity;

	// Kepler Orbit Elements
	double semiMajorAxis;
	float eccentricity;
	float inclination = 0.0f;
	float ascLongitude = 0.0f;
	float argPeriapsis = 0.0f;
	float trueAnomaly = 0.0f;

	// Helper Elements
	float meanAnomaly = 0.0f;
	double n = 0.0;
	float period = 0.0f;
	float currentPeriod = 0.0f;

	// Render Properties
	int size;
	uint32_t color;
	Vec3Double orbitPoints[32];

public:
	CelestialBody() = default;
	~CelestialBody() = default;

	void CalculateParametersAndElements(const CelestialBody& M)
	{
		double u = G * (M.mass + mass);
		period = (2 * M_PI) * sqrt(pow(semiMajorAxis, 3) / u);

		n = sqrt((G * M.mass) / pow(semiMajorAxis, 3));

		inclination = (inclination / 180) * M_PI;
		ascLongitude = (ascLongitude / 180) * M_PI;
		argPeriapsis = (argPeriapsis / 180) * M_PI;
	}

	void CalculateOrbitPoints(const double systemSize)
	{
		double min = systemSize / 2;

		double a;
		double r;

		float x;

		/*position.y =
			r * sin(argPeriapsis + a) * sin(inclination);*/

		float z;

		for (size_t i = 0; i < 32; i += 1)
		{
			a = (static_cast<float>(i) / 32) * (M_PI * 2);
			r = (semiMajorAxis * (1 - pow(eccentricity, 2))) /
				(1 + eccentricity * cos(a));

			x = r * (cos(ascLongitude) * cos(argPeriapsis + a) -
				sin(ascLongitude) * sin(argPeriapsis + a) * cos(inclination));

			/*position.y =
			r * sin(argPeriapsis + a) * sin(inclination);*/

			z = r * (sin(ascLongitude) * cos(argPeriapsis + a) +
				cos(ascLongitude) * sin(argPeriapsis + a) * cos(inclination));

			orbitPoints[i] = Vec3Double(x, 0.0f, z);
		}
	}
};