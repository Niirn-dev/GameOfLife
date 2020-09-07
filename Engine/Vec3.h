#pragma once

#include "Vec2.h"

template <typename T>
class Vec3_
{
public:
	Vec3_() = default;
	Vec3_( T x,T y,T w = (T)1 )
		:
		x( x ),
		y( y ),
		w( w )
	{
	}

	template <typename S>
	explicit Vec3_( const Vec3_<S>& src )
		:
		x( T( src.x ) ),
		y( T( src.y ) ),
		w( T( src.w ))
	{
	}

	explicit Vec3_( const Vec2_<T>& src )
		:
		x( src.x ),
		y( src.y ),
		w( (T)1 )
	{
	}
	explicit operator Vec2_<T>() const
	{
		return { x,y };
	}

	Vec3_	operator-() const
	{
		return { -x,-y };
	}
	Vec3_	operator+( const Vec3_& rhs ) const
	{
		return Vec3_( x + rhs.x,y + rhs.y );
	}
	Vec3_&	operator+=( const Vec3_& rhs )
	{
		return *this = *this + rhs;
	}
	Vec3_	operator*( T rhs ) const
	{
		return Vec3_( x * rhs,y * rhs );
	}
	Vec3_&	operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	Vec3_	operator-( const Vec3_& rhs ) const
	{
		return Vec3_( x - rhs.x,y - rhs.y );
	}
	Vec3_&	operator-=( const Vec3_& rhs )
	{
		return *this = *this - rhs;
	}
	Vec3_	operator/( T rhs ) const
	{
		return { x / rhs,y / rhs };
	}
	Vec3_&	operator/=( T rhs )
	{
		return *this = *this / rhs;
	}
	bool	operator==( const Vec3_& rhs ) const
	{
		return ( x == rhs.x ) && ( y == rhs.y );
	}
	bool	operator!=( const Vec3_& rhs ) const
	{
		return !( *this == rhs );
	}

	T		GetLength() const
	{
		return (T)std::sqrt( GetLengthSq() );
	}
	T		GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec3_&	Normalize()
	{
		*this = GetNormalized();
	}
	Vec3_	GetNormalized() const
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
	T w = (T)1;
};

using Vec3 = Vec3_<float>;
using Vei3 = Vec3_<int>;
