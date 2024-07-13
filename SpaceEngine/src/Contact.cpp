#include "Contact.h"

void Contact::ResolvePenetration()
{
	if (a->IsStatic() && b->IsStatic()) return;

	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	if(a->resolvePentration)
		a->position -= normal * da;
	if(b->resolvePentration)
		b->position += normal * db;
}

void Contact::ResolveCollision()
{
	ResolvePenetration();

	float e = std::min(a->restitution, b->restitution);

	const Vec2 vRel = (a->velocity - b->velocity);

	float vRelDotNormal = vRel.Dot(normal);

	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * vRelDotNormal / (a->invMass + b->invMass);

	Vec2 j = impulseDirection * impulseMagnitude;

	if (a->resolvePentration)
		a->ApplyImpulse(j);
	if (b->resolvePentration)
		b->ApplyImpulse(j);
}
