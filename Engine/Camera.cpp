#include "Camera.h"

Camera::Camera( CoordinatesTransformer& ct )
    :
    ct( ct )
{
}

void Camera::DrawRect( RectF rect,Color c ) const
{
    rect.Translate( -pos );
    rect.Scale( scale );
    ct.DrawRect( std::move( rect ),c );
}

void Camera::Draw( Drawable drawable ) const
{
    drawable.Translate( -pos );
    drawable.Scale( scale );
    ct.Draw( std::move( drawable ) );
}

void Camera::SetScale( float s )
{
    scale = s;
}

float Camera::GetScale() const
{
    return scale;
}

void Camera::SetPosition( const Vec2& pos_in )
{
    pos = pos_in;
}

void Camera::MoveBy( const Vec2& offset )
{
    pos += offset;
}

const Vec2& Camera::GetPosition() const
{
    return pos;
}

bool Camera::ContainsDrawable( const Drawable& drawable ) const
{
    auto screenRect = ct.GetScreenRect();
    screenRect.Scale( 1.0f / scale );
    screenRect.Translate( pos );

    return screenRect.IsOverlappingWith( drawable.GetRect() );
}
