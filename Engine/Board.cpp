#include "Board.h"
#include <algorithm>
#include <iterator>
#include <random>
#include <cassert>

Board::Cell::Cell( const Vec2& pos,Color c )
	:
	pos( pos ),
	c( c )
{
}

void Board::Cell::SetModel( std::vector<Vec2> model_in )
{
	model = std::move( model_in );
}

Board::Cell::Cell( const Vec2& pos,Color c,std::vector<Vec2> model )
	:
	model( std::move( model ) ),
	pos( pos ),
	c( c )
{
}

Board::Cell Board::Cell::MakeDefault( const Vec2& pos,Color c )
{
	std::vector<Vec2> model;
	model.reserve( 4 );
	model.push_back( { -radius + 2.0f,-radius + 2.0f } );
	model.push_back( { -radius + 2.0f,+radius - 2.0f } );
	model.push_back( { +radius - 2.0f,+radius - 2.0f } );
	model.push_back( { +radius - 2.0f,-radius + 2.0f } );
	return std::move( Cell( pos,c,std::move( model ) ) );
}

void Board::Cell::SetScale( float s )
{
	scale = s;
}

float Board::Cell::GetScale() const
{
	return scale;
}

Drawable Board::Cell::GetDrawable() const
{
	Drawable drawable( model,c,std::move( GetRect() ) );
	drawable.Scale( scale );
	drawable.Translate( pos );
	return std::move( drawable );
}

Board::Star::Star( const Vec2& pos,Color c,float outerRadius,float radiiRatio,int nFlares,float angleOffset )
	:
	Cell( pos,c ),
	outerRadius( std::min( outerRadius,GetRadius() ) ),
	innerRadius( outerRadius * std::min( radiiRatio,1.0f / radiiRatio ) ),
	nFlares( nFlares )
{
	assert( angleOffset >= -3.14159f );
	assert( angleOffset <= 3.14159f );

	std::vector<Vec2> star;
	star.reserve( 2 * size_t( nFlares ) );
	const float dTheta = 2.0f * 3.14159f / float( 2 * nFlares );
	for ( int i = 0; i < 2 * nFlares; ++i )
	{
		const float rad = ( i % 2 == 0 ) ? outerRadius : innerRadius;
		star.emplace_back(
			rad * cosf( dTheta * float( i ) + angleOffset ),
			rad * sinf( dTheta * float( i ) + angleOffset )
		);
	}
	SetModel( std::move( star ) );
}


void Board::Cell::SetColor( Color c_in )
{
	c = c_in;
}

void Board::Cell::ToggleState()
{
	isAlive = !isAlive;
}

bool Board::Cell::IsAlive() const
{
	return isAlive;
}

RectF Board::Cell::GetRect() const
{
	return RectF( pos - Vec2{ GetRadius(),GetRadius() },pos + Vec2{ GetRadius(),GetRadius() } );
}

Board::Board( float width,float height )
	:
	width( width ),
	height( height ),
	rect( -width / 2.0f,width / 2.0f,height / 2.0f,-height / 2.0f ),
	nCellsAcross( size_t( width / Cell::GetSize() ) ),
	nCellsUp( size_t( height / Cell::GetSize() ) )
{
	const Color colors[] = {
		Colors::White,
		Colors::Red,
		Colors::Cyan,
		Colors::Green,
		Colors::Yellow,
		Colors::Gray
	};
	const auto nColors = std::end( colors ) - std::begin( colors );

	std::mt19937 rng( std::random_device{}() );
	std::uniform_int_distribution<int> colorDist( 0,nColors - 1 );
	std::uniform_real_distribution<float> orDist( minOuterRadius,maxOuterRadius );
	std::uniform_real_distribution<float> rrDist( minRatio,maxRatio );
	std::uniform_int_distribution<int> flareDist( minFlares,maxFlares );
	std::uniform_real_distribution<float> angleDist( minAngle,maxAngle );

	int i = 0;
	cells.reserve( nCellsAcross * nCellsUp );
	for ( Vec2 pos = { rect.left + Cell::GetRadius(),rect.bottom + Cell::GetRadius() }; pos.y < rect.top; pos.y += Cell::GetSize() )
	{
		for ( pos.x = rect.left + Cell::GetRadius(); pos.x < rect.right; pos.x += Cell::GetSize() )
		{
			cells.push_back( std::make_unique<Star>( 
				pos,
				colors[colorDist( rng )],
				orDist( rng ),
				rrDist( rng ),
				flareDist( rng ),
				angleDist( rng )
			) );
			cells.back()->ToggleState();
			++i;
		}
	}
}

std::vector<Drawable> Board::GetDrawables() const
{
	std::vector<Drawable> drawables;

	for ( const auto& c : cells )
	{
		if ( c->IsAlive() )
		{
			drawables.push_back( std::move( c->GetDrawable() ) );
		}
	}

	return std::move( drawables );
}

RectF Board::GetRect() const
{
	return rect;
}