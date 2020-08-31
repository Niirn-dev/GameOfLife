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
	void SetAngle( float a );
	float GetAngle() const;
	const Vec2& GetPosition() const;
	bool ContainsDrawable( const Drawable& drawable ) const;

private:
	CoordinatesTransformer& ct;
	Vec2 pos = { 0.0f,0.0f };
	float scale = 1.0f;
	float angle = 0.0f;
};

