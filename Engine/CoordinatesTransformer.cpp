#include "CoordinatesTransformer.h"
#include "Mat3.h"

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

void CoordinatesTransformer::Draw( Drawable drawable ) const
{
	const Vec2 offset = Vec2{ float( Graphics::ScreenWidth / 2 ),float( Graphics::ScreenHeight / 2 ) };
	drawable.ApplyTransformation(
		Mat3::Translate( offset ) *
		Mat3::Scale( 1.0f,-1.0f )
	);
	drawable.Render( gfx );
}

RectF CoordinatesTransformer::GetScreenRect() const
{
	const Vec2 offset = Vec2{ float( Graphics::ScreenWidth / 2 ),float( Graphics::ScreenHeight / 2 ) };
	return RectF( -offset,offset );
}
