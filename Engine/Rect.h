#pragma once

#include "Vec2.h"

template <typename T>
class Rect_
{
public:
	Rect_() = default;
	Rect_( T left,T right,T top,T bottom )
		:
		left( left ),
		right( right ),
		top( top ),
		bottom( bottom )
	{
	}
	Rect_( const Vec2_<T>& topLeft,const Vec2_<T> bottomRight )
		:
		Rect_( topLeft,bottomRight.x - topLeft.x,bottomRight.y - topLeft.y )
	{
	}
	Rect_( const Vec2_<T>& topLeft,T width,T height )
		:
		Rect_( topLeft.x,topLeft.x + width,topLeft.y,topLeft.y + height )
	{
	}

	static Rect_ FromCenter( const Vec2_<T>& center,T halfWidth,T halfHeight )
	{
		const auto half = Vec2_<T>( halfWidth,halfHeight );
		return Rect_( center - half,center + half );
	}
	Rect_ GetExpanded( T offset ) const
	{
		return Rect_( left - offset,right + offset,top - offset,bottom + offset );
	}

	bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right && 
			bottom > other.top && top < other.bottom;
	}
	bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	bool Contains( const Vec2_<T>& point ) const
	{
		return point.x >= left && point.x < right&& point.y >= top && point.y < bottom;
	}

	Vec2_<T> GetCenter() const
	{
		return Vec2_<T>( ( left + right ) / T( 2 ),( top + bottom ) / T( 2 ) );
	}
	T GetWidth() const
	{
		return right - left;
	}
	T GetHeight() const
	{
		return bottom - top;
	}

public:
	T left;
	T right;
	T top;
	T bottom;
};

using RectF = Rect_<float>;
using RectI = Rect_<int>;
