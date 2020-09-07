#include "Drawable.h"

Drawable::Drawable( const std::vector<Vec2>& model,Color c,RectF boundRect )
	:
	c( c ),
	model( model ),
	boundRect( std::move( boundRect ) )
{
}

void Drawable::ApplyTransformation( const Mat3& transformation_in )
{
	transformation = transformation_in * transformation;
}

void Drawable::Render( Graphics& gfx )
{
	gfx.DrawClosedPolyline( model,c,transformation );
}

const RectF& Drawable::GetRect() const
{
	return boundRect;
}
