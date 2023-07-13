#include "Shape.h"

CircleShape::CircleShape(const float radius)
{
	this->radius = radius;
}

CircleShape::~CircleShape()
{
	
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(this->radius);
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5f * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
}

PolygonShape::~PolygonShape()
{
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(this->localVertices);
}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

float PolygonShape::GetMomentOfInertia() const
{
	return 0.0f;
}

void PolygonShape::UpdateVertices(const float& rotation, const Vec2& position)
{
	for (int i = 0; i < localVertices.size(); i++)
	{
		worldVertices[i] = localVertices[i].Rotate(rotation);
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(float width, float height)
{
	this->width = width;
	this->height = height;

	localVertices.push_back(Vec2(-width * 0.5f, -height * 0.5f));
	localVertices.push_back(Vec2(width * 0.5f, -height * 0.5f));
	localVertices.push_back(Vec2(width * 0.5f, height * 0.5f));
	localVertices.push_back(Vec2(-width * 0.5f, height * 0.5f));

	worldVertices = localVertices;
}

BoxShape::~BoxShape()
{
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(this->width, this->height);
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

float BoxShape::GetMomentOfInertia() const
{
	return (0.083333) * (width * width + height * height);
}
