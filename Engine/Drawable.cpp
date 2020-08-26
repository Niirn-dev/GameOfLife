#include "Drawable.h"

Drawable::Drawable( RectF model,Color c )
	:
	c( c ),
	model( std::move( model ) )
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
	/*for ( auto& v : model )
	{
		v.x *= scale_x;
		v.y *= scale_y;
		v += translation;
	}
	gfx.DrawClosedPolyline( model,c );*/
	model.Scale( scale_x,scale_y );
	model.Translate( translation );
	gfx.DrawRect( model.left,model.right,model.top,model.bottom,c );
}
