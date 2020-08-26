#pragma once

#include "Vec2.h"
#include <vector>

/// <summary>
/// Rectagle in math axis ( y positive direction is up )
/// </summary>
/// <typeparam name="T"></typeparam>
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
	Rect_( const Vec2_<T>& bottomLeft,const Vec2_<T> topRight )
		:
		Rect_( bottomLeft,topRight.x - bottomLeft.x,topRight.y - bottomLeft.y )
	{
	}
	Rect_( const Vec2_<T>& bottomLeft,T width,T height )
		:
		Rect_( bottomLeft.x,bottomLeft.x + width,bottomLeft.y + height,bottomLeft.y )
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
		return Rect_( left - offset,right + offset,top + offset,bottom - offset );
	}

	inline bool IsOverlappingWith( const Rect_& other ) const
	{
		return right > other.left && left < other.right && 
			bottom < other.top && top > other.bottom;
	}
	inline bool IsContainedBy( const Rect_& other ) const
	{
		return left >= other.left && right <= other.right &&
			top <= other.top && bottom >= other.bottom;
	}
	inline bool Contains( const Vec2_<T>& point ) const
	{
		return point.x >= left && point.x <= right && point.y >= bottom && point.y <= top;
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
		return top - bottom;
	}
	std::vector<Vec2_<T>> GetVeritices() const
	{
		std::vector<Vec2_<T>> verts( 4 );
		verts.push_back( { left,bottom } );
		verts.push_back( { left,top } );
		verts.push_back( { right,top } );
		verts.push_back( { right,bottom } );
		return std::move( verts );
	}

public:
	T left;
	T right;
	T top;
	T bottom;
};

using RectF = Rect_<float>;
using RectI = Rect_<int>;
