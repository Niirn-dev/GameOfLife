/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <vector>
#include "Vec2.h"
#include <random>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	ct( gfx ),
	cam( ct ),
	camCtrl( wnd.mouse,wnd.kbd,cam ),
	mouseIn( wnd.mouse,cam,ct ),
	brd( boardWidth,boardHeight )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	ParseInput( dt );

	brd.Update( dt );
}

void Game::ParseInput( float dt )
{
	// Handle continious keyboard states
	camCtrl.UpdateKeyboard( dt );

	// Handle keyboard events
	while ( !wnd.kbd.KeyIsEmpty() )
	{
		auto e = wnd.kbd.ReadKey();
		camCtrl.UpdateKeyboardEvent( e,dt );
		if ( e.IsPress() && e.GetCode() == VK_SPACE )
		{
			brd.OnPauseClick();
		}
	}

	// Handle mouse events
	while ( !wnd.mouse.IsEmpty() )
	{
		auto e = wnd.mouse.Read();
		camCtrl.UpdateMouseEvent( e,dt );
		if ( e.GetType() == Mouse::Event::Type::RPress )
		{
			auto mpos = mouseIn.GetMousePos( e );
			if ( brd.GetRect().Contains( mpos ) )
			{
				brd.OnToggleCellStateClick( mpos );
			}
		}
	}
}

void Game::ComposeFrame()
{
	std::vector<Drawable> drawables = std::move( brd.GetDrawables() );
	for ( auto d : drawables )
	{
		if ( cam.ContainsDrawable( d ) )
		{
			cam.Draw( std::move( d ) );
		}
	}

	drawables = std::move( brd.GetBorderDrawables() );
	for ( auto d : drawables )
	{
		cam.Draw( std::move( d ) );
	}
}
