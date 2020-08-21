#include "Board.h"
#include <cassert>
#include <algorithm>
#include <iterator>

Board::Board( const std::vector<Vei2> gridPos )
	:
	grid( width * height )
{
	// Initializing living cells
	for ( const auto& gp : gridPos )
	{
		assert( gp.x >= 0 );
		assert( gp.x < width );
		assert( gp.y >= 0 );
		assert( gp.y < height );
		// Make sure we're not toggling the same cell twice
		if ( !grid[GridToIndex( gp )] )
		{
			grid[GridToIndex( gp )] = true;
			aliveCellsPos.push_back( gp );
		}
	}
}

void Board::Update()
{
	std::unordered_set<Vei2> changedStates;
	// Mark cells that are up for toggling
	{
		std::vector<Vei2> aliveNextGen;
		std::copy_if(
			aliveCellsPos.begin(),aliveCellsPos.end(),
			std::back_inserter( aliveNextGen ),
			[&]( const Vei2& pos ) 
			{
				// Figure out the future state of dead neighbors in the next generation
				for ( int y = std::max( 0,pos.y - 1 ); y <= std::min( height - 1,pos.y + 1 ); ++y )
				{
					for ( int x = std::max( 0,pos.x - 1 ); x <= std::min( width - 1,pos.x + 1 ); ++x )
					{
						Vei2 target = { x,y };
						assert( GetRect().Contains( target ) );
						// Make sure the cell is dead
						// This works because the wasChecked map is cleared every generation
						if ( !grid[GridToIndex( target )] )
						{
							if ( CountAliveNeighbors( target ) == 3 )
							{
								changedStates.insert( target );
							}
						}
					}
				}

				// Figure out if the cell dies in the next generation
				if ( int neighbs = CountAliveNeighbors( pos ); neighbs < 2 || neighbs > 3 )
				{
					changedStates.insert( pos );
					// Don't copy if it's gonna be dead
					return false;
				}
				return true;
			}
		);
		// Put old alive cells that'll stay alive back in the tracking vector
		aliveCellsPos = std::move( aliveNextGen );
	}

	// Toggle states for all marked cells and cleanup
	for ( const auto& pos : changedStates )
	{
		grid[GridToIndex( pos )] = !grid[GridToIndex( pos )];
		// Add new alive cells to the vector
		if ( grid[GridToIndex( pos )] )
		{
			aliveCellsPos.push_back( pos );
		}
	}
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
				count += grid[GridToIndex( target )];
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
		topLeft.x + gridPos.x * cellSize,
		topLeft.y + gridPos.y * cellSize
	};;
}

Vei2 Board::ScreenToGrid( const Vei2& screenPos ) const
{
	assert( GetRect().Contains( screenPos ) );

	return Vei2{
		( screenPos.x - topLeft.x ) / cellSize,
		( screenPos.y - topLeft.y ) / cellSize
	};
}

size_t Board::GridToIndex( const Vei2& gridPos ) const
{
	return (size_t)gridPos.y * width + gridPos.x;
}

RectI Board::GetCellRect( const Vei2& gridPos ) const
{
	return RectI{
		GridToScreen( gridPos ),
		cellSize,
		cellSize
	};
}
