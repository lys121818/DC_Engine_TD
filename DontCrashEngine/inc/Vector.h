#pragma once

namespace DC_Engine
{
	struct Vector2
	{
	public:
		int x = 0;
		int y = 0;

		Vector2(int x, int y) :x(x), y(y) {};
		Vector2(const Vector2& vec) = default;
		Vector2(Vector2&& vec) = default;

		Vector2& operator=(const Vector2& other);
		Vector2& operator+(const Vector2& other);
		Vector2& operator-(const Vector2& other);

	};

	struct Vector2_f
	{
	public:
		float x = 0;
		float y = 0;

		Vector2_f() {};
		Vector2_f(float x, float y) :x(x), y(y) {};
		Vector2_f(const Vector2_f& vec) = default;
		Vector2_f(Vector2_f&& vec) = default;

		Vector2_f& operator=(const Vector2_f& other);
		Vector2_f& operator+(const Vector2_f& other);
		Vector2_f& operator-(const Vector2_f& other);

		float GetDistance(const Vector2_f& other) const;
		float GetAngle(const Vector2_f& other) const;

		Vector2_f Normalize(const Vector2_f& other) const; // Todo : returns the 
		static Vector2_f NormalizeAngle(const float& angle) ;
	};
}

