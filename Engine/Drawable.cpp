#include "Drawable.h"

Drawable::Drawable( const std::vector<Vec2>& model,Color c,RectF boundRect )
	:
	c( c ),
	model( model ),
	boundRect( std::move( boundRect ) )
{
}

void Drawable::Translate( const Vec2& offset )
{
	translation += offset;
}

void Drawable::Scale( float factor )
{
	scale_x *= factor;
	scale_y *= factor;
	translation *= factor;
}

void Drawable::ScaleIndividually( float factor_x,float factor_y )
{
	scale_x *= factor_x;
	scale_y *= factor_y;
	translation.x *= factor_x;
	translation.y *= factor_y;
}

void Drawable::Render( Graphics& gfx )
{
	gfx.DrawClosedPolyline( model,c,translation,scale_x,scale_y );
}

const RectF& Drawable::GetRect() const
{
	return boundRect;
}
