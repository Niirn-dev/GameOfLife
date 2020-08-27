#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Graphics.h"

class Drawable
{
public:
	Drawable( const std::vector<Vec2>& model,Color c,RectF boundRect );

	void Translate( const Vec2& offset );
	void Scale( float factor );
	void ScaleIndividually( float factor_x,float factor_y );
	void Render( Graphics& gfx );
	const RectF& GetRect() const;

private:
	Color c;
	const std::vector<Vec2>& model;
	RectF boundRect;
	Vec2 translation = { 0.0f,0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
};

