#include "stdafx.h"
#include "Math.h"

namespace BVMath
{
	BVPoint::BVPoint()
	{
		x = y = 0.0f;
	}
	BVPoint::~BVPoint()
	{

	}
	BVPoint::BVPoint(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	BVRect::BVRect()
	{
		top = left = bottom = right = 0.0f;
	}
	BVRect::~BVRect()
	{

	}
	BVRect::BVRect(float _left, float _top, float _right, float _bottom)
	{
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}
	BVRect::BVRect(BVPoint _topleft, BVPoint _bottomright)
	{
		top = _topleft.y;
		left = _topleft.x;
		bottom = _bottomright.y;
		right = _bottomright.x;
	}
	BVRect::BVRect(RECT rect)
	{
		top = (float)rect.top;
		left = (float)rect.left;
		bottom = (float)rect.bottom;
		right = (float)rect.right;
	}
	D2D1_RECT_F BVRect::D2D()
	{
		D2D1_RECT_F ret;
		ret.top = top;
		ret.bottom = bottom;
		ret.left = left;
		ret.right = right;
		return ret;
	}

	bool CheckCollision(BVRect rect, BVPoint point)
	{
		return ((point.x >= rect.left && point.x < rect.right) && (point.y >= rect.top && point.y < rect.bottom));
	}
}
