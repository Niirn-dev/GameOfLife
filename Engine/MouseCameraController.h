#pragma once

#include "Mouse.h"
#include "Camera.h"
#include "Vec2.h"

class MouseCameraController
{
public:
	MouseCameraController( Mouse& mouse,Camera& camera )
		:
		mouse( mouse ),
		cam( camera )
	{
	}
	void Update()
	{
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

			cam.MoveBy( deltaPos / cam.GetScale() );
			prevPos = curMousePos;
		}
	}

private:
	Mouse& mouse;
	Camera& cam;
	static constexpr float scaleFactor = 1.05f;
	Vei2 prevPos = { 0,0 };
	bool isEngaged = false;
};
