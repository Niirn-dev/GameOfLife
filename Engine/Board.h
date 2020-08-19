#pragma once

#include "Graphics.h"
#include "Vec2.h"
#include <vector>
#include <queue>
#include <unordered_map>

class Board
{
private:
	class Cell
	{
	public:
		enum class State
		{
			Dead,
			Alive
		};

		bool IsAlive() const;
		void ToggleState();

	private:
		State state = State::Dead;
	public:
		static constexpr int size = 5;
	};
public:
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
	/// <param name="gfx"></param>
	void Draw( Graphics& gfx ) const;

private:
	int CountAliveNeighbors( const Vei2& gridPos ) const;
	Vei2 GridToScreen( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos ) const;

	RectI GetCellRect( const Vei2& gridPos ) const;

private:
	static constexpr Vei2 topLeft = { };
	static constexpr int width = Graphics::ScreenWidth / Cell::size;
	static constexpr int height = Graphics::ScreenHeight / Cell::size;
	std::unordered_map<Vei2,Cell> grid;

	std::queue<Vei2> upForToggling;
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
			width * Cell::size,
			height * Cell::size
		};
	}
};

