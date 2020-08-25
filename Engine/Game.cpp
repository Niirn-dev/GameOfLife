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
	pBrd( std::make_unique<Board>() ),
	rect( 0.0f,50.0f,0.0f,50.0f )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	// UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	// Manage player keyboard
	while ( !wnd.kbd.KeyIsEmpty() )
	{
		auto e = wnd.kbd.ReadKey();
		if ( e.IsPress() )
		{
			switch ( e.GetCode() )
			{
			case VK_SPACE:
				isSimulationPaused = !isSimulationPaused;
				break;
			case 'R':
				pBrd.reset();
				pBrd = std::make_unique<Board>();
				isSimulationPaused = true;
				break;
			default:
				break;
			}
		}
	}

	if ( !isSimulationPaused )
	{
		elapsedTime += dt;
		while ( elapsedTime >= stepTime )
		{
			pBrd->Update();

			elapsedTime -= stepTime;
		}
	}
	else
	{
		// Manage player mouse inputs while paused
		// Process events one at a time
		while ( !wnd.mouse.IsEmpty() )
		{
			auto e = wnd.mouse.Read();
			if ( e.GetType() == Mouse::Event::Type::LPress )
			{
				pBrd->ToggleCellState( e.GetPos() );
			}
		}
		// Machine gun events
		if ( wnd.mouse.RightIsPressed() )
		{
			pBrd->ToggleCellState( wnd.mouse.GetPos() );
		}
	}
}

void Game::ComposeFrame()
{
	/*
	if ( !isSimulationPaused )
	{
		pBrd->Draw( gfx,Colors::White );
	}
	else
	{
		pBrd->Draw( gfx,{ 225,180,225 } );
	}
	*/
	if ( !wnd.kbd.KeyIsEmpty() )
	{
		Vec2 offset = { 0.0f,0.0f };
		if ( wnd.kbd.KeyIsPressed( VK_LEFT ) )
		{
			offset.x += -5.0f;
		}
		if ( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
		{
			offset.x += 5.0f;
		}
		if ( wnd.kbd.KeyIsPressed( VK_UP ) )
		{
			offset.y += 5.0f;
		}
		if ( wnd.kbd.KeyIsPressed( VK_DOWN ) )
		{
			offset.y += -5.0f;
		}
		rect.Translate( offset );
	}

	ct.DrawRect( rect,Colors::White );
}
