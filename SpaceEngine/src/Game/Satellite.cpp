#include "Satellite.h"
#include "../Phsyics/Force.h"
#include "../Phsyics/PhysicsConstants.h"

Satellite::Satellite()
{
}

Satellite::Satellite(const float& x, const float& y, const float& radius, const float& mass, const float& soi)
{
	body = new Body(CircleShape(radius), x, y, mass);
	sphereOfInfluence = soi;
}

Satellite::~Satellite()
{
	
}

void Satellite::Update(const float& dt, const Satellite* satellite)
{
	if (satellite)
	{
		Vec2 attraction = Force::GenerateGravitationalForce(*body, *satellite->GetBody(), G, 0, 100);
		body->AddForce(attraction);
	}

	body->Update(dt);
}

void Satellite::Render()
{
}
