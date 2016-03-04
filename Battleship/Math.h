#pragma once
#include <d2d1.h> 

namespace BVMath
{
	class BVPoint
	{
	public:
		BVPoint();
		~BVPoint();
		BVPoint(float _x, float _y);

		float x, y;
	};

	class BVRect
	{
	public:
		BVRect();
		~BVRect();
		BVRect(float _left, float _top, float _right, float _bottom);
		BVRect(BVPoint _topleft, BVPoint _bottomright);
		BVRect(RECT rect);
		D2D1_RECT_F D2D();

		float left, top, right, bottom;
	};

	bool CheckCollision(BVRect rect, BVPoint point);
}

