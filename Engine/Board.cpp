#include "Board.h"
#include <algorithm>
#include <iterator>

Board::Cell::Cell( Vec2 pos,Color c )
	:
	pos( pos ),
	c( c )
{
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
	Drawable drawable( GetRect(),c );
	drawable.Scale( scale );
	return std::move( drawable );
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

	int i = 0;
	cells.reserve( nCellsAcross * nCellsUp );
	for ( Vec2 pos = { rect.left + Cell::GetRadius(),rect.bottom + Cell::GetRadius() }; pos.y < rect.top; pos.y += Cell::GetSize() )
	{
		for ( pos.x = rect.left + Cell::GetRadius(); pos.x < rect.right; pos.x += Cell::GetSize() )
		{
			cells.emplace_back( pos,colors[i % nColors] );
			cells.back().ToggleState();
			++i;
		}
	}
}

std::vector<Drawable> Board::GetDrawables() const
{
	std::vector<Drawable> drawables;

	for ( const auto& c : cells )
	{
		if ( c.IsAlive() )
		{
			drawables.push_back( std::move( c.GetDrawable() ) );
		}
	}

	return std::move( drawables );
}

RectF Board::GetRect() const
{
	return rect;
}
