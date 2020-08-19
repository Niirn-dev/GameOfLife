#pragma once

#include <cmath>

template <typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_( T x,T y )
		:
		x( x ),
		y( y )
	{
	}

	template <typename S>
	explicit Vec2_( const Vec2_<S>& src )
		:
		x( T( src.x ) ),
		y( T( src.y ) )
	{
	}

	Vec2_  operator+( const Vec2_& rhs ) const
	{
		return Vec2_( x + rhs.x,y + rhs.y );
	}
	Vec2_& operator+=( const Vec2_& rhs )
	{
		return *this = *this + rhs;
	}
	Vec2_  operator*( float rhs ) const
	{
		return Vec2_( x * rhs,y * rhs );
	}
	Vec2_& operator*=( float rhs )
	{
		return *this = *this * rhs;
	}
	Vec2_  operator-( const Vec2_& rhs ) const
	{
		return Vec2_( x - rhs.x,y - rhs.y );
	}
	Vec2_& operator-=( const Vec2_& rhs )
	{
		return *this = *this - rhs;
	}

	T GetLength() const
	{
		return (T)std::sqrt( GetLengthSq() );
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec2_& Normalize()
	{
		*this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if ( len != T( 0 ) )
		{
			return *this * ( T( 1 ) / len );
		}
		return *this;
	}
public:
	T x;
	T y;
};

using Vec2 = Vec2_<float>;
using Vei2 = Vec2_<int>;
