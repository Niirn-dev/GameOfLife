#include "Camera.h"

Camera::Camera( CoordinatesTransformer& ct )
    :
    ct( ct )
{
}

void Camera::DrawRect( RectF rect,Color c )
{
    rect.Translate( -pos );
    rect.Scale( scale );
    ct.DrawRect( std::move( rect ),c );
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
