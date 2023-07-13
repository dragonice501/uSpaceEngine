#pragma once

#include "Vec2.h"

#include <vector>

enum ShapeType
{
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape
{
	virtual ~Shape() = default;

	virtual Shape* Clone() const = 0;

	virtual ShapeType GetType() const = 0;
	virtual float GetMomentOfInertia() const = 0;
};

struct CircleShape: public Shape
{
	float radius;

	CircleShape(const float radius);
	virtual ~CircleShape();

	Shape* Clone() const override;

	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
};

struct PolygonShape: public Shape
{
	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;

	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2> vertices);
	virtual ~PolygonShape();

	Shape* Clone() const override;

	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;

	virtual void UpdateVertices(const float& rotation, const Vec2& position);
};

struct BoxShape : public PolygonShape
{
	float width;
	float height;

	BoxShape(float width, float height);
	virtual ~BoxShape();

	Shape* Clone() const override;

	ShapeType GetType() const override;
	float GetMomentOfInertia() const override;
};