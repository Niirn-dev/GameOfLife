#pragma once

#include "Mouse.h"
#include "Camera.h"
#include "Vec2.h"
#include "Keyboard.h"
#include "MiscMath.h"

class CameraController
{
public:
	CameraController( Mouse& mouse,Keyboard& kbd,Camera& camera )
		:
		mouse( mouse ),
		kbd( kbd ),
		cam( camera )
	{
	}
	void Update( float dt )
	{
		// Handle keyboard stuff
		if ( kbd.KeyIsPressed( 'Q' ) )
		{
			cam.SetAngle( cam.GetAngle() - rotSpeed * dt );
		}
		if ( kbd.KeyIsPressed( 'E' ) )
		{
			cam.SetAngle( cam.GetAngle() + rotSpeed * dt );
		}
		while ( !kbd.KeyIsEmpty() )
		{
			auto e = kbd.ReadKey();
			if ( e.IsPress() && e.GetCode() == 'R' )
			{
				cam.Reset();
			}
		}

		// Handle mousee stuff
		while ( !mouse.IsEmpty() )
		{
			const auto e = mouse.Read();
			switch ( e.GetType() )
			{
			case Mouse::Event::Type::LPress:
				prevPos = e.GetPos();
				isEngaged = true;
				break;
			case Mouse::Event::Type::LRelease:
				isEngaged = false;
				break;
			case Mouse::Event::Type::WheelUp:
				cam.SetScale( cam.GetScale() * scaleFactor );
				break;
			case Mouse::Event::Type::WheelDown:
				cam.SetScale( cam.GetScale() / scaleFactor);
				break;
			default:
				break;
			}
		}
		if ( isEngaged )
		{
			const Vei2 curMousePos = mouse.GetPos();
			Vec2 deltaPos = Vec2( curMousePos - prevPos );
			deltaPos.x *= -1.0f;
			deltaPos.Rotate( cam.GetAngle() );

			cam.MoveBy( deltaPos / cam.GetScale() );
			prevPos = curMousePos;
		}
	}

private:
	Mouse& mouse;
	Keyboard& kbd;
	Camera& cam;
	static constexpr float scaleFactor = 1.05f;
	static constexpr float rotSpeed = PI / 4.0f;
	Vei2 prevPos = { 0,0 };
	bool isEngaged = false;
};
