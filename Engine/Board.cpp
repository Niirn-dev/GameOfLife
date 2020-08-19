#include "Board.h"
#include <cassert>
#include <algorithm>

Board::Cell::Cell( State s )
	:
	state( s )
{
}

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
			grid[gp] = Cell( Cell::State::Dead );
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
		if ( !grid[gp].IsAlive() )
		{
			grid[gp] = Cell( Cell::State::Alive );
			aliveCellsPos.push_back( gp );
		}
	}
}

void Board::Update()
{
	// Mark cells that are up for toggling
	{
		auto new_end = std::remove_if(
			aliveCellsPos.begin(),aliveCellsPos.end(),
			[&]( const Vei2& pos ) 
			{
				// Figure out the future state of dead neighbors in the next generation
				for ( int y = std::max( 0,pos.y - 1 ); y <= std::min( height - 1,pos.y + 1 ); ++y )
				{
					for ( int x = std::max( 0,pos.x - 1 ); x <= std::min( width - 1,pos.x + 1 ); ++x )
					{
						Vei2 target = { x,y };
						assert( GetRect().Contains( target ) );
						// Make sure the cell is dead and wasn't checked yet
						// This works because the wasChecked map is cleared every generation
						if ( !grid[target].IsAlive() &&
							 ( wasChecked.find( target ) == wasChecked.end() ) )
						{
							wasChecked[target] = true;
							if ( CountAliveNeighbors( target ) == 3 )
							{
								upForToggling.push( pos );
							}
						}
					}
				}

				// Figure out the state of this cell in the next generation
				if ( int neighbs = CountAliveNeighbors( pos ); neighbs < 2 || neighbs > 3 )
				{
					upForToggling.push( pos );
					// Remove from vector if the cell is gonna be dead
					return true;
				}
				return false;
			}
		);
		aliveCellsPos.erase( new_end,aliveCellsPos.end() );
	}

	// Toggle states for all marked cells and cleanup
	for ( ; !upForToggling.empty(); upForToggling.pop() )
	{
		grid[upForToggling.front()].ToggleState();
		// Add new alive cells to the vector
		if ( grid[upForToggling.front()].IsAlive() )
		{
			aliveCellsPos.push_back( upForToggling.front() );
		}
	}
	wasChecked.clear();
}

void Board::Draw( Graphics& gfx ) const
{
	// Iterate over alive cells
	for ( auto& pos : aliveCellsPos )
	{
		gfx.DrawRect( GetCellRect( pos ),Colors::White );
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
