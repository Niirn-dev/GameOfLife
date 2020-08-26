#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Colors.h"
#include "Drawable.h"
#include <memory>

class Board
{
private:
	class Cell
	{
	protected:
		Cell( const Vec2& pos,Color c );
		void SetModel( std::vector<Vec2> model_in );
	private:
		Cell( const Vec2& pos,Color c,std::vector<Vec2> model );
	public:
		static Cell MakeDefault( const Vec2& pos,Color c );
		virtual ~Cell() = default;

		void SetScale( float s );
		float GetScale() const;
		virtual Drawable GetDrawable() const;
		void SetColor( Color c_in );
		void ToggleState();
		bool IsAlive() const;
	private:
		RectF GetRect() const;

	private:
		static constexpr float radius = 20.0f;
		std::vector<Vec2> model;
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

	class Star : public Cell
	{
	public:
		Star( const Vec2& pos,Color c,float outerRadius,float radiiRatio,int nFlares,float angleOffset );

	private:
		const float outerRadius;
		const float innerRadius;
		const int nFlares;
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
	std::vector<std::unique_ptr<Cell>> cells;

	// Parameters for cell stars generation
	static constexpr float maxOuterRadius	= 18.0f;
	static constexpr float minOuterRadius	= 10.0f;
	static constexpr float maxRatio			= 0.9f;
	static constexpr float minRatio			= 0.4f;
	static constexpr int   maxFlares		= 16;
	static constexpr int   minFlares		= 3;
	static constexpr float maxAngle			= 3.14159f;
	static constexpr float minAngle			= -3.14159f;
};

