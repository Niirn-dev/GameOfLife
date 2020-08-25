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
	template<typename T2>
	explicit operator Rect_<T2>() const
	{
		return { T2( left ),T2( right ),T2( top ),T2( bottom ) };
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

	inline bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right && 
			bottom > other.top && top < other.bottom;
	}
	inline bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	inline bool Contains( const Vec2_<T>& point ) const
	{
		return point.x >= left && point.x < right&& point.y >= top && point.y < bottom;
	}

	void Translate( const Vec2_<T>& offset )
	{
		left += offset.x;
		right += offset.x;
		top += offset.y;
		bottom += offset.y;
	}
	void Scale( T factor )
	{
		left *= factor;
		right *= factor;
		top *= factor;
		bottom *= factor;
	}
	void Scale( T factor_x,T factor_y )
	{
		left *= factor_x;
		right *= factor_x;
		top *= factor_y;
		bottom *= factor_y;
	}

	inline Vec2_<T> GetCenter() const
	{
		return Vec2_<T>( ( left + right ) / T( 2 ),( top + bottom ) / T( 2 ) );
	}
	inline T GetWidth() const
	{
		return right - left;
	}
	inline T GetHeight() const
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
