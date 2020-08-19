#include "Board.h"
#include <cassert>
#include <algorithm>

bool Board::Cell::IsAlive() const
{
	return state == State::Alive;
}

void Board::Cell::ToggleState()
{
	state = (State)( (int)state ^ (int)State::Alive );
}

Board::Board( const std::vector<Vei2> gridPos )
	:
	grid( width * height )
{
	// Initialize entire map?
	for ( Vei2 gp = { 0,0 }; gp.y < height; ++gp.y )
	{
		for ( gp.x = 0; gp.x < width; ++gp.x )
		{
			grid[gp] = Cell();
		}
	}

	// Initializing living cells
	for ( const auto& gp : gridPos )
	{
		assert( gp.x >= 0 );
		assert( gp.x < width );
		assert( gp.y >= 0 );
		assert( gp.y < height );
		// Make sure we're not toggling the same cell twice
		// if can't find the cell -> wrong coordinates, do nothing
		if ( auto target = grid.find( gp ); 
			 target != grid.end() && !target->second.IsAlive() )
		{
			target->second.ToggleState();
		}
	}
}

void Board::Update()
{
	// Mark cells that are up for toggling
	for ( Vei2 gp = { 0,0 }; gp.y < height; ++gp.y )
	{
		for ( gp.x = 0; gp.x < width; ++gp.x )
		{
			int neighbs = CountAliveNeighbors( gp );
			if ( grid[gp].IsAlive() )
			{
				if ( neighbs < 2 || neighbs > 3 )
				{
					upForToggling.push( gp );
				}
			}
			else
			{
				if ( neighbs == 3 )
				{
					upForToggling.push( gp );
				}
			}
		}
	}

	// Toggle states for all marked cells
	for ( ; !upForToggling.empty(); upForToggling.pop() )
	{
		grid[upForToggling.front()].ToggleState();
	}
}

void Board::Draw( Graphics& gfx ) const
{
	for ( auto& p : grid )
	{
		if ( p.second.IsAlive() )
		{
			gfx.DrawRect( GetCellRect( p.first ),Colors::White );
		}
	}
}

int Board::CountAliveNeighbors( const Vei2& gridPos ) const
{
	int count = 0;
	for ( int y = std::max( 0,gridPos.y - 1 ); y <= std::min( height - 1,gridPos.y + 1 ); ++y )
	{
		for ( int x = std::max( 0,gridPos.x - 1 ); x <= std::min( width - 1,gridPos.x + 1 ); ++x )
		{
			Vei2 target = { x,y };
			assert( GetRect().Contains( target ) );
			if ( target != gridPos )
			{
				count += grid.find(target)->second.IsAlive();
			}
		}
	}
	return count;
}

Vei2 Board::GridToScreen( const Vei2& gridPos ) const
{
	assert( gridPos.x >= 0 );
	assert( gridPos.x < width );
	assert( gridPos.y >= 0 );
	assert( gridPos.y < height );

	return Vei2{
		topLeft.x + gridPos.x * Cell::size,
		topLeft.y + gridPos.y * Cell::size
	};;
}

Vei2 Board::ScreenToGrid( const Vei2& screenPos ) const
{
	assert( GetRect().Contains( screenPos ) );

	return Vei2{
		( screenPos.x - topLeft.x ) / Cell::size,
		( screenPos.y - topLeft.y ) / Cell::size
	};
}

RectI Board::GetCellRect( const Vei2& gridPos ) const
{
	return RectI{
		GridToScreen( gridPos ),
		Cell::size,
		Cell::size
	};
}
