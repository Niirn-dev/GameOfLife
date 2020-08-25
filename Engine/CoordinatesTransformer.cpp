#include "CoordinatesTransformer.h"

CoordinatesTransformer::CoordinatesTransformer( Graphics& gfx )
	:
	gfx( gfx )
{
}

void CoordinatesTransformer::DrawRect( RectF rect,Color c ) const
{
	const Vec2 offset = Vec2{ float( Graphics::ScreenWidth / 2 ),float( Graphics::ScreenHeight / 2 ) };
	rect.Scale( 1.0f,-1.0f );
	rect.Translate( offset );
	gfx.DrawRect( rect.left,rect.right,rect.top,rect.bottom,c );
}
