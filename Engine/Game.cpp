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
	gfx( wnd )
{
	std::mt19937 rng( std::random_device{}() );
	std::uniform_int_distribution<int> xDist( 0,Board::GetWidth() - 1 );
	std::uniform_int_distribution<int> yDist( 0,Board::GetHeight() - 1 );

	std::vector<Vei2> pos;
	for ( int i = 0; i < 500; ++i )
	{
		pos.emplace_back( xDist( rng ),yDist( rng ) );
	}
	pBrd = std::make_unique<Board>( pos );
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
	elapsedTime += ft.Mark();
	while ( elapsedTime >= stepTime )
	{
		pBrd->Update();
		elapsedTime -= stepTime;
	}
}

void Game::ComposeFrame()
{
	pBrd->Draw( gfx );
}
