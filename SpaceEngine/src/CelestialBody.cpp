#include "CelestialBody.h"
#include "PhysicsConstants.h"

#include <math.h>

void CelestialBody::CalculateParametersAndElements(const CelestialBody& M)
{
	double u = G * (M.mass + mass);
	period = (2 * PI) * sqrt(pow(semiMajorAxis, 3) / u);

	n = sqrt((G * M.mass) / pow(semiMajorAxis, 3));

	inclination = (inclination / 180) * PI;
	ascLongitude = (ascLongitude / 180) * PI;
	argPeriapsis = (argPeriapsis / 180) * PI;
}

void CelestialBody::CalculateOrbitPoints(const double systemSize)
{
	double min = systemSize / 2;
	double a;
	double r;

	float x;
	float z;

	for (size_t i = 0; i < 32; i += 1)
	{
		a = (static_cast<float>(i) / 32) * (PI * 2);
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
