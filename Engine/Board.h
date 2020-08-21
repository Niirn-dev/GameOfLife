#pragma once

#include "Graphics.h"
#include "Vec2.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

class Board
{
public:
	/// <summary>
	/// Default constructor for an empty board
	/// </summary>
	Board();
	/// <summary>
	/// Constructs the board with the defined alive cells
	/// </summary>
	/// <param name="pos"> grid positions of the alive cells </param>
	Board( const std::vector<Vei2> gridPos );
	/// <summary>
	/// Progresses universe one step forward
	/// </summary>
	void Update();
	/// <summary>
	/// Draws all living cells to the screen
	/// </summary>
	/// <param name="gfx">reference of a graphics object</param>
	/// /// <param name="c">color of the alive cells</param>
	void Draw( Graphics& gfx,Color c ) const;

	void ToggleCellState( const Vei2& screenPos );

private:
	int CountAliveNeighbors( const Vei2& gridPos ) const;
	Vei2 GridToScreen( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos ) const;

	size_t GridToIndex( const Vei2& gridPos ) const;

	RectI GetCellRect( const Vei2& gridPos ) const;

private:
	static constexpr Vei2 topLeft = { };
	static constexpr int cellSize = 5;
	static constexpr int width = Graphics::ScreenWidth / cellSize;
	static constexpr int height = Graphics::ScreenHeight / cellSize;
	std::vector<bool> grid;

	std::vector<Vei2> aliveCellsPos;
public:
	static constexpr int GetWidth()
	{
		return width;
	}
	static constexpr int GetHeight()
	{
		return height;
	}
	static RectI GetRect()
	{
		return RectI{
			topLeft,
			width * cellSize,
			height * cellSize
		};
	}
};

