#include "Satellite.h"
#include "Force.h"
#include "PhysicsConstants.h"
#include "Graphics.h"
#include "Font.h"

Satellite::Satellite()
{
}

Satellite::Satellite(const char* newName, const float& x, const float& y, const float& radius, const float& mass, const float& soi, Satellite* influencingSatellite)
{
	body = new Body(CircleShape(radius), x, y, mass);
	this->influencingSatellite = influencingSatellite;

	sphereOfInfluence = soi;
	
	int i = 0;
	while (newName[i] != '\0')
	{
		name[i] = newName[i];
		i++;
	}
}

Satellite::~Satellite()
{
	delete body;

	influencingSatellite = nullptr;
}

void Satellite::Update(const float& dt)
{
	if (influencingSatellite)
	{
		Vec2 attraction = Force::GenerateGravitationalForce(*body, *influencingSatellite->GetBody(), G, 0, influencingSatellite->GetSOI());
		body->AddForce(attraction);
	}

	body->Update(dt);
}

void Satellite::Render()
{
	if (body->shape->GetType() == CIRCLE)
	{
		CircleShape*  circle = static_cast<CircleShape*>(body->shape);
		Graphics::DrawFillCircle(body->position.x, body->position.y, circle->radius, color);
	}

	Graphics::DrawCircle(body->position.x, body->position.y, sphereOfInfluence, 0.0f, color, false);

	Graphics::DrawString(
		body->position.x - Font::GetStringFontLength(name) * 0.5f,
		body->position.y - Font::fontHeight * 0.5f,
		name,
		0xFF000000, false);
}
