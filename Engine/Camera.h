#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "CoordinatesTransformer.h"
#include "Mouse.h"
#include <vector>
#include "Drawable.h"

class Camera
{
public:
	Camera( CoordinatesTransformer& ct );
	void DrawRect( RectF rect,Color c ) const;
	void Draw( Drawable drawable ) const;

public:
	void SetScale( float s );
	float GetScale() const;
	void SetPosition( const Vec2& pos_in );
	void MoveBy( const Vec2& offset );
	const Vec2& GetPosition() const;

private:
	CoordinatesTransformer& ct;
	Vec2 pos = { 0.0f,0.0f };
	float scale = 1.0f;
};

