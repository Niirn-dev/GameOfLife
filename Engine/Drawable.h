#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Graphics.h"

class Drawable
{
public:
	Drawable( RectF model,Color c );

	void Translate( const Vec2& offset );
	void Scale( float factor );
	void ScaleIndividually( float factor_x,float factor_y );
	void Render( Graphics& gfx );
	RectF GetRect() const;

private:
	Color c;
	RectF model;
	Vec2 translation = { 0.0f,0.0f };
	float scale_x = 1.0f;
	float scale_y = 1.0f;
};

