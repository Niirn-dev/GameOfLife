#pragma once

#include "Vec2.h"
#include "Vec3.h"

template<typename T>
class Mat3_
{
public:
	Vec2_<T>	operator*( const Vec2_<T>& v ) const
	{
		return Vec2_<T>( *this * (Vec3_<T>)v );
	}
	Vec3_<T>	operator*( const Vec3_<T>& v ) const
	{
		Vec3_<T> vout;
		vout.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.w;
		vout.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.w;
		vout.w = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.w;
		return vout;
	}

	Mat3_		operator*( const Mat3_& rhs ) const
	{
		Mat3_ mout;
		for ( int row_left = 0; row_left < 3; ++row_left )
		{
			for ( int col_right = 0; col_right < 3; ++col_right )
			{
				mout.cells[row_left][col_right] = (T)0;
				for ( int i = 0; i < 3; ++i )
				{
					mout.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return mout;
	}

	static Mat3_ Translate( const Vec2_<T>& offset )
	{
		return {
			(T)1,	(T)0,	(T)offset.x,
			(T)0,	(T)1,	(T)offset.y,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3_ Scale( T factor )
	{
		return {
			(T)factor,	(T)0,		(T)0,
			(T)0,		(T)factor,	(T)0,
			(T)0,		(T)0,		(T)1
		};
	}
	static Mat3_ Scale( T factor_x,T factor_y )
	{
		return {
			(T)factor_x,(T)0,		(T)0,
			(T)0,		(T)factor_y,(T)0,
			(T)0,		(T)0,		(T)1
		};
	}
	static Mat3_ Rotate( T angle )
	{
		const auto sint = std::sin( angle );
		const auto cost = std::cos( angle );
		return {
			cost,	-sint,	(T)0,
			sint,	cost,	(T)0,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3_ Identity()
	{
		return Scale( (T)1 );
	}

public:
	// cells[row][col]
	T cells[3][3];
};

using Mat3 = Mat3_<float>;
