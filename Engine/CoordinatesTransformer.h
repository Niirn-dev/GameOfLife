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
	RectF GetScreenRect() const;
	Mat3 GetTransformation() const;
	Mat3 GetTransformationInverse() const;

private:
	Graphics& gfx;
};

