#include "Satellite.h"
#include "../Phsyics/Force.h"
#include "../Phsyics/PhysicsConstants.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Font.h"

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
	if (body->shape->GetType() == CIRCLE)
	{
		CircleShape*  circle = static_cast<CircleShape*>(body->shape);
		Graphics::DrawFillCircle(body->position.x, body->position.y, circle->radius, color);
	}

	Graphics::DrawCircle(body->position.x, body->position.y, sphereOfInfluence, 0.0f, color, false);

	Graphics::DrawString(
		body->position.x - Font::GetStringFontLength("tulli") * 0.5f,
		body->position.y - Font::fontHeight * 0.5f,
		"tulli",
		0xFF000000, false);
}
