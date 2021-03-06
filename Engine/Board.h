#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Colors.h"
#include "Drawable.h"
#include <memory>
#include <unordered_set>
#include "MiscMath.h"

class Board
{
private:
	class Cell
	{
	protected:
		Cell( const Vec2& pos,Color c,float rotSpeed = 0.0f );
		void SetModel( std::vector<Vec2> model_in );
	private:
		void GenerateSelectionModels();
		Cell( const Vec2& pos,Color c,std::vector<Vec2> model,float rotSpeed = 0.0f );
	public:
		static Cell MakeDefault( const Vec2& pos,Color c );
		virtual ~Cell() = default;

		void SetScale( float s );
		float GetScale() const;

		void SetAngle( float a );
		float GetAngle() const;

		void SetColor( Color c_in );

		void ToggleState();
		bool IsAlive() const;
		bool IsTransitioning() const;

		void ToggleSelection();
		bool IsSelected() const;

		Vec2 GetPos() const;

		Drawable GetDrawable() const;
		std::vector<Drawable> GetSelectionDrawables() const;

		/// <summary>
		/// Returns true if the cell finished transitioning and is finally dead
		/// </summary>
		bool Update( const Board& brd,float dt );
	private:
		void UpdateRotation( const Board& brd,float dt );
	public:
		bool AnimateTransition( const Board& brd,float dt );
	private:
		RectF GetRect() const;

	private:
		static constexpr float radius = 20.0f;
		std::vector<Vec2> model;
		Vec2 pos; ///< Center of the cell
		float scale = 1.0f;
		float angle = 0.0f;
		float rotSpeed;
		Color c;
		bool isAlive = false;
		bool isTransitioning = false;

		bool isSelected = false;
		static constexpr int selectionThickness = 2;
		static constexpr Color selectionColor = Colors::Red;
		std::vector<std::vector<Vec2>> selectionModels;

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
		Star( const Vec2& pos,Color c,float outerRadius,float radiiRatio,int nFlares,float angleOffset,float rotSpeed = 0.0f );

	private:
		const float outerRadius;
		const float innerRadius;
		const int nFlares;
	};

public:
	Board( float width,float height,float stepDuration = 1.0f );
private:
	void GenerateBorderModel();

public:
	void Update( float dt );
private:
	void UpdateBoardState();
	void UpdateCells( float dt );

public:
	void OnToggleCellStateClick( const Vec2& screenPos );
	void OnPauseClick();
	std::vector<Drawable> GetDrawables() const;
	RectF GetRect() const;
	std::vector<Drawable> GetBorderDrawables() const;
private:
	void AppendCellSelectionDrawables( std::vector<Drawable>& drawables ) const;

private:
	int CountAliveNeighbors( const Cell* target ) const;
	int CellToIndex( const Cell* cp ) const;
	const Cell& CellAt( int x,int y ) const;
	Cell& CellAt( int x,int y );
	Cell& CellAtScreen( const Vec2& screenPos );

private:
	const float stepDuration = 1.0f;
	float stepTime = 0.0f;
	bool isPaused = false;
	const float width;
	const float height;
	const RectF rect;
	const int nCellsAcross;
	const int nCellsUp;
	std::vector<std::unique_ptr<Cell>> cellPtrs;
	std::vector<Cell*> aliveCellPtrs;
	std::unordered_set<Cell*> updateCellPtrs;

	static constexpr int borderThickness = 3;
	std::vector<std::vector<Vec2>> borderModels;
	static constexpr Color borderColor = Colors::Gray;

	// Parameters for cell stars generation
	static constexpr float maxOuterRadius	= 18.0f;
	static constexpr float minOuterRadius	= 10.0f;
	static constexpr float maxRatio			= 0.6f;
	static constexpr float minRatio			= 0.2f;
	static constexpr int   maxFlares		= 16;
	static constexpr int   minFlares		= 3;
	static constexpr float maxAngle			= PI;
	static constexpr float minAngle			= -PI;
	static constexpr float minRotSpeed		= -1.5f;
	static constexpr float maxRotSpeed		= 1.5f;
};

