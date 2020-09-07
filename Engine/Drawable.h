#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include "Graphics.h"
#include "Mat3.h"

class Drawable
{
public:
	Drawable( const std::vector<Vec2>& model,Color c,RectF boundRect );

	void ApplyTransformation( const Mat3& transformation_in );
	void Render( Graphics& gfx );
	const RectF& GetRect() const;

private:
	Color c;
	const std::vector<Vec2>& model;
	RectF boundRect;
	Mat3 transformation = Mat3::Identity();
};

