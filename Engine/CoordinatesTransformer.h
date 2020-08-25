#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Graphics.h"

class CoordinatesTransformer
{
public:
	CoordinatesTransformer( Graphics& gfx );
	void DrawRect( RectF rect,Color c ) const;

private:
	Graphics& gfx;
};

