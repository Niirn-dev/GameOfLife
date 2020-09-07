#include "Board.h"
#include <algorithm>
#include <iterator>
#include <random>
#include <cassert>
#include <functional>
#include "Mat3.h"

Board::Cell::Cell( const Vec2& pos,Color c,float rotSpeed/*= 0.0f*/ )
	:
	pos( pos ),
	c( c ),
	rotSpeed( rotSpeed )
{
	GenerateSelectionModels();
}

void Board::Cell::SetModel( std::vector<Vec2> model_in )
{
	model = std::move( model_in );
}

void Board::Cell::GenerateSelectionModels()
{
	selectionModels.resize( selectionThickness );
	for ( int i = 0; i < selectionThickness; ++i )
	{
		selectionModels.reserve( 4 );
		selectionModels[i] = std::move( GetRect().GetExpanded( -float( i + 1 ) ).GetVeritices() );
	}
}

Board::Cell::Cell( const Vec2& pos,Color c,std::vector<Vec2> model,float rotSpeed/* = 0.0f*/ )
	:
	model( std::move( model ) ),
	pos( pos ),
	c( c ),
	rotSpeed( rotSpeed )
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

void Board::Cell::SetAngle( float a )
{
	angle = a;
}

float Board::Cell::GetAngle() const
{
	return angle;
}

void Board::Cell::SetColor( Color c_in )
{
	c = c_in;
}

void Board::Cell::ToggleState()
{
	isAlive = !isAlive;
	isTransitioning = true;
}

bool Board::Cell::IsAlive() const
{
	return isAlive;
}

bool Board::Cell::IsTransitioning() const
{
	return isTransitioning;
}

void Board::Cell::ToggleSelection()
{
	isSelected = !isSelected;
}

bool Board::Cell::IsSelected() const
{
	return isSelected;
}

Vec2 Board::Cell::GetPos() const
{
	return pos;
}

Drawable Board::Cell::GetDrawable() const
{
	Drawable drawable( model,c,std::move( GetRect() ) );
	drawable.ApplyTransformation(
		Mat3::Translate( pos ) *
		Mat3::Rotate( angle ) *
		Mat3::Scale( scale )
	);
	return std::move( drawable );
}

std::vector<Drawable> Board::Cell::GetSelectionDrawables() const
{
	std::vector<Drawable> drawables;
	drawables.reserve( selectionThickness );
	for ( int i = 0; i < selectionThickness; ++i )
	{
		drawables.emplace_back( selectionModels[i],selectionColor,GetRect() );
	}

	return drawables;
}

bool Board::Cell::Update( const Board& brd,float dt )
{
	UpdateRotation( brd,dt );
	if ( isTransitioning )
	{
		return AnimateTransition( brd,dt );
	}
	// if we got here that means the cell should be alive because we shouldn't update dead cells that finished transitioning
	assert( isAlive );
	return false;
}

void Board::Cell::UpdateRotation( const Board& brd,float dt )
{
	SetAngle( GetAngle() + rotSpeed * dt );
}

bool Board::Cell::AnimateTransition( const Board& brd,float dt )
{
	if ( isAlive )
	{
		SetScale( std::min( 1.5f * brd.stepTime / brd.stepDuration,1.0f ) );
	}
	else
	{
		SetScale( std::max( -1.5f * brd.stepTime / brd.stepDuration + 1.0f,0.0f ) );
	}

	if ( brd.stepTime >= ( brd.stepDuration / 1.5f ) )
	{
		isTransitioning = false;
		return !isAlive;
	}
	return false;
}

RectF Board::Cell::GetRect() const
{
	return RectF( pos - Vec2{ GetRadius(),GetRadius() },pos + Vec2{ GetRadius(),GetRadius() } );
}

Board::Star::Star( const Vec2& pos,Color c,float outerRadius,float radiiRatio,int nFlares,float angleOffset,float rotSpeed/*= 0.0f*/ )
	:
	Cell( pos,c,rotSpeed ),
	outerRadius( std::min( outerRadius,GetRadius() ) ),
	innerRadius( outerRadius* std::min( radiiRatio,1.0f / radiiRatio ) ),
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

Board::Board( float width,float height,float stepDuration/*= 1.0f*/ )
	:
	stepDuration( stepDuration ),
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
	std::uniform_real_distribution<float> rotSpeedDist( minRotSpeed,maxRotSpeed );

	std::uniform_int_distribution<int> spawnDist( 0,100 );
	cellPtrs.reserve( (size_t)nCellsAcross * nCellsUp );
	for ( Vec2 pos = { rect.left + Cell::GetRadius(),rect.bottom + Cell::GetRadius() }; pos.y < rect.top; pos.y += Cell::GetSize() )
	{
		for ( pos.x = rect.left + Cell::GetRadius(); pos.x < rect.right; pos.x += Cell::GetSize() )
		{
			cellPtrs.push_back( std::make_unique<Star>(
				pos,
				colors[colorDist( rng )],
				orDist( rng ),
				rrDist( rng ),
				flareDist( rng ),
				angleDist( rng ),
				rotSpeedDist( rng )
			) );
			if ( spawnDist( rng ) > 75 )
			{
				cellPtrs.back()->ToggleState();
				aliveCellPtrs.push_back( cellPtrs.back().get() );
				updateCellPtrs.insert( cellPtrs.back().get() );
			}
		}
	}

	GenerateBorderModel();
}

void Board::GenerateBorderModel()
{
	borderModels.resize( borderThickness );
	for ( int i = 0; i < borderThickness; ++i )
	{
		borderModels.reserve( 4 );
		borderModels[i] = std::move( rect.GetExpanded( float( i + 1 ) ).GetVeritices() );
	}
}

void Board::Update( float dt )
{
	if ( isPaused )
	{
		return;
	}

	stepTime += dt;
	if ( stepTime >= stepDuration )
	{
		UpdateBoardState();

		stepTime -= stepDuration;
	}

	UpdateCells( dt );
}

void Board::UpdateBoardState()
{
	std::unordered_set<Cell*> changedStates;
	// Mark cells that are up for toggling
	std::vector<Cell*> aliveNextGeneration;
	std::copy_if(
		aliveCellPtrs.begin(),aliveCellPtrs.end(),
		std::back_inserter( aliveNextGeneration ),
		[&]( Cell* pc )
		{
			// Get index of the cell
			const int center = CellToIndex( pc );
			const int center_y = center / nCellsAcross;
			const int center_x = center - center_y * nCellsAcross;

			// Figure out the future state of dead neighbors in the next generation
			for ( auto y = std::max( 0,center_y - 1 ); y <= std::min( (int)nCellsUp - 1,center_y + 1 ); ++y )
			{
				for ( auto x = std::max( 0,center_x - 1 ); x <= std::min( (int)nCellsAcross - 1,center_x + 1 ); ++x )
				{
					if ( !CellAt( x,y ).IsAlive() )
					{
						if ( CountAliveNeighbors( &CellAt( x,y ) ) == 3 )
						{
							changedStates.insert( &CellAt( x,y ) );
						}
					}
				}
			}

			// Figure out if the cell dies in the next generation
			if ( int neighbs = CountAliveNeighbors( pc ); neighbs < 2 || neighbs > 3 )
			{
				changedStates.insert( pc );
				return false;
			}
			return true;
		}
	);
	aliveCellPtrs = aliveNextGeneration;


	// Toggle states for all marked cells and cleanup
	for ( const auto& pc : changedStates )
	{
		pc->ToggleState();
		// Add new alive cells to the vector
		if ( pc->IsAlive() )
		{
			aliveCellPtrs.push_back( cellPtrs[CellToIndex( pc )].get() );
		}
		// Update all cells that changed states
		updateCellPtrs.insert( pc );
	}
}

void Board::UpdateCells( float dt )
{
	for ( auto it = updateCellPtrs.begin(); it != updateCellPtrs.end(); ) {
		if ( ( *it )->Update( *this,dt ) ) 
		{
			it = updateCellPtrs.erase( it );
		}
		else 
		{
			++it;
		}
	}
}

void Board::OnToggleCellStateClick( const Vec2& screenPos )
{
	Cell& cell = CellAtScreen( screenPos );
	cell.ToggleState();
	if ( isPaused )
	{
		cell.ToggleSelection();
	}
	updateCellPtrs.insert( &cell );
	if ( cell.IsAlive() )
	{
		aliveCellPtrs.push_back( &cell );
	}
	else
	{
		aliveCellPtrs.erase( std::remove( aliveCellPtrs.begin(),aliveCellPtrs.end(),&cell ),aliveCellPtrs.end() );
	}
}

void Board::OnPauseClick()
{
	isPaused = !isPaused;

	if ( !isPaused )
	{
		for ( auto& pc : updateCellPtrs )
		{
			if ( pc->IsSelected() )
			{
				pc->ToggleSelection();
			}
		}
	}
}

std::vector<Drawable> Board::GetDrawables() const
{
	std::vector<Drawable> drawables;

	for ( const auto& pc : updateCellPtrs )
	{
		assert( pc->IsAlive() || pc->IsTransitioning() );
		drawables.push_back( std::move( pc->GetDrawable() ) );
	}

	if ( isPaused )
	{
		AppendCellSelectionDrawables( drawables );
	}

	return std::move( drawables );
}

RectF Board::GetRect() const
{
	return rect;
}

std::vector<Drawable> Board::GetBorderDrawables() const
{
	std::vector<Drawable> drawables;
	drawables.reserve( borderThickness );
	for ( int i = 0; i < borderThickness; ++i )
	{
		drawables.emplace_back( borderModels[i],borderColor,rect );
	}

	return drawables;
}

void Board::AppendCellSelectionDrawables( std::vector<Drawable>& drawables ) const
{
	assert( isPaused );
	std::vector<Drawable> selections;
	for ( const auto& pc : updateCellPtrs )
	{
		if ( pc->IsSelected() )
		{
			selections = std::move( pc->GetSelectionDrawables() );
			std::copy( std::make_move_iterator( selections.begin() ),std::make_move_iterator( selections.end() ),std::back_inserter( drawables ) );
		}
	}
}

int Board::CountAliveNeighbors( const Cell* target ) const
{
	const int center = CellToIndex( target );
	const int center_y = center / nCellsAcross;
	const int center_x = center - center_y * nCellsAcross;

	int count = 0;
	for ( auto y = std::max( 0,center_y - 1 ); y <= std::min( (int)nCellsUp - 1,center_y + 1 ); ++y )
	{
		for ( auto x = std::max( 0,center_x - 1 ); x <= std::min( (int)nCellsAcross - 1,center_x + 1 ); ++x )
		{
			count += CellAt( x,y ).IsAlive();
		}
	}
	count -= CellAt( center_x,center_y ).IsAlive();

	return count;
}

int Board::CellToIndex( const Cell* cp ) const
{
	Vec2 relPos = cp->GetPos() - Vec2{ rect.left,rect.bottom };
	int x = int( relPos.x / Cell::GetSize() );
	int y = int( relPos.y / Cell::GetSize() );
	assert( x >= 0 );
	assert( x < nCellsAcross );
	assert( y >= 0 );
	assert( y < nCellsUp );
	return y * nCellsAcross + x;
}

const Board::Cell& Board::CellAt( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < nCellsAcross );
	assert( y >= 0 );
	assert( y < nCellsUp );
	return *cellPtrs[y * (size_t)nCellsAcross + x];
}

Board::Cell& Board::CellAt( int x,int y )
{
	assert( x >= 0 );
	assert( x < nCellsAcross );
	assert( y >= 0 );
	assert( y < nCellsUp );
	return *cellPtrs[y * (size_t)nCellsAcross + x];
}

Board::Cell& Board::CellAtScreen( const Vec2& screenPos )
{
	assert( rect.Contains( screenPos ) );
	Vec2 relPos = screenPos - Vec2{ rect.left,rect.bottom };
	int x = int( relPos.x / Cell::GetSize() );
	int y = int( relPos.y / Cell::GetSize() );
	return CellAt( x,y );
}
