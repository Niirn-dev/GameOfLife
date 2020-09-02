#pragma once

#include "Camera.h"
#include "CoordinatesTransformer.h"
#include "Mouse.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Mat3.h"

#include <sstream>

class MouseInputTransformer
{
public:
	MouseInputTransformer( Mouse& m,const Camera& cam,const CoordinatesTransformer& ct )
		:
		mouse( m ),
		cam( cam ),
		ct( ct )
	{
	}

	Vec2 GetMousePos()
	{
		Mat3 transf = cam.GetTransformation() * ct.GetTransformation();
		Vec2 mpos = transf * (Vec2)mouse.GetPos();
		return std::move( mpos );
	}
	Vec2 GetMousePos( Mouse::Event e )
	{
		Mat3 transf = cam.GetTransformationInverse() * ct.GetTransformationInverse();
		Vec2 mpos = (Vec2)e.GetPos();
		mpos = transf * mpos;

		std::wstringstream wss;
		wss << L"Converted mouse coordinates were: { x: " << mpos.x << ",y: " << mpos.y << " }" << std::endl;
		OutputDebugStringW( wss.str().c_str() );

		return std::move( mpos );
	}

private:
	Mouse& mouse;
	const Camera& cam;
	const CoordinatesTransformer& ct;
};
