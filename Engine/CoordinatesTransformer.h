#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Graphics.h"
#include "Drawable.h"

class CoordinatesTransformer
{
public:
	CoordinatesTransformer( Graphics& gfx );
	void DrawRect( RectF rect,Color c ) const;
	void Draw( Drawable drawable ) const;

private:
	Graphics& gfx;
};

