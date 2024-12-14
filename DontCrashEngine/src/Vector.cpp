#include "Vector.h"

// Include from STL
#include <cmath>

namespace DC_Engine
{
	Vector2& Vector2::operator=(const Vector2& other)
	{

		if (this != &other)	// avoid self assign
		{
			if (other.x != x || other.y != y)
			{
				x = other.x;
				y = other.y;
			}
		}
		return *this;
	}

	Vector2& Vector2::operator+(const Vector2& other)
	{
		if (this != &other)	// avoid self assign
		{
			x += other.x;
			y += other.y;
		}
		return *this;
	}

	Vector2& Vector2::operator-(const Vector2& other)
	{
		if (this != &other)	// avoid self assign
		{
			x -= other.x;
			y -= other.y;
		}
		return *this;
	}

	Vector2_f& Vector2_f::operator=(const Vector2_f& other)
	{
		if (this != &other)	// avoid self assign
		{
			if (other.x != x || other.y != y)
			{
				x = other.x;
				y = other.y;
			}
		}
		return *this;
	}

	Vector2_f& Vector2_f::operator+(const Vector2_f& other)
	{
		if (this != &other)	// avoid self assign
		{
			x += other.x;
			y += other.y;
		}
		return *this;
	}

	Vector2_f& Vector2_f::operator-(const Vector2_f& other)
	{
		if (this != &other)	// avoid self assign
		{
			x -= other.x;
			y -= other.y;
		}
		return *this;
	}

	float Vector2_f::GetDistance(const Vector2_f& other) const
	{
		float delta_x = std::abs(x - other.x);
		float delta_y = std::abs(y - other.y);

		delta_x *= delta_x;
		delta_y *= delta_y;

		float distance = std::sqrt(delta_x + delta_y);
		
		return distance;
	}

	float Vector2_f::GetAngle(const Vector2_f& other) const
	{
		float delta_x = other.x - x;
		float delta_y = other.y - y;

		float angle = atan2(delta_y, delta_x);
		
		const double pi = std::atan(1.0) * 4;

		angle *= 180 / (float)pi;
		return angle;
	}

	Vector2_f Vector2_f::Normalize(const Vector2_f& other) const
	{
		float vectorLength = GetDistance(other);
		Vector2_f vectorNorm;
		
		vectorNorm.x = x / vectorLength;
		vectorNorm.y = y / vectorLength;

		return vectorNorm;
	}
	
	Vector2_f Vector2_f::NormalizeAngle(const float& angle) 
	{
		Vector2_f angleNorm;

		const double pi = std::atan(1.0) * 4;

		float radian = (float)pi / 180;

		angleNorm.x = cos(radian * angle);
		angleNorm.y = sin(radian * angle);

		return angleNorm;
	}
}

