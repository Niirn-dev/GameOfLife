#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Colors.h"
#include "Drawable.h"

class Board
{
private:
	class Cell
	{
	public:
		Cell( Vec2 pos,Color c );

		void SetScale( float s );
		float GetScale() const;
		Drawable GetDrawable() const;
		void SetColor( Color c_in );
		void ToggleState();
		bool IsAlive() const;
	private:
		RectF GetRect() const;

	private:
		static constexpr float radius = 2.5f;
		Vec2 pos; ///< Center of the cell
		float scale = 1.0f;
		Color c;
		bool isAlive = false;

	public:
		static float GetSize()
		{
			return radius * 2.0f;
		}
		static float GetRadius()
		{
			return radius;
		}
	};

public:
	Board( float width,float height );

	std::vector<Drawable> GetDrawables() const;
	RectF GetRect() const;

private:
	const float width;
	const float height;
	const RectF rect;
	const size_t nCellsAcross;
	const size_t nCellsUp;
	std::vector<Cell> cells;
};

