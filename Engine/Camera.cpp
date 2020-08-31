#include "Camera.h"
#include "Mat3.h"
#include "MiscMath.h"

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
    drawable.ApplyTransformation(
        Mat3::Rotate( -angle ) *
        Mat3::Scale( scale ) *
        Mat3::Translate( -pos )
    );
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

void Camera::SetAngle( float a )
{
    angle = a;
}

float Camera::GetAngle() const
{
    return angle;
}

const Vec2& Camera::GetPosition() const
{
    return pos;
}

bool Camera::ContainsDrawable( const Drawable& drawable ) const
{
    const auto zoom = 1.0f / scale;
    const auto rad = std::sqrt(
        sq( float( Graphics::ScreenWidth / 2 ) * zoom ) +
        sq( float( Graphics::ScreenHeight / 2 ) * zoom )
    );
    auto screenRect = RectF::FromCenter( pos,rad,rad );
    return screenRect.IsOverlappingWith( drawable.GetRect() );
}
