#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Character = new Character(m_ViewPort);
}

void Game::Cleanup( )
{
	delete m_Character;
}

void Game::Update( float elapsedSec )
{
	m_Character->Update(elapsedSec);
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );
	//m_Character->DrawBoard();
	m_Character->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_a:
		m_Character->SetDirection(Directions::left);
		break;
	case SDLK_d:
		m_Character->SetDirection(Directions::right);
		break;
	case SDLK_w:
		m_Character->SetDirection(Directions::up);
		break;
	case SDLK_s:
		m_Character->SetDirection(Directions::down);
		break;
	case SDLK_RIGHT:
		m_Character->SetDirection(Directions::right);
		break;
	case SDLK_LEFT:
		m_Character->SetDirection(Directions::left);
		break;
	case SDLK_UP:
		m_Character->SetDirection(Directions::up);
		break;
	case SDLK_DOWN:
		m_Character->SetDirection(Directions::down);
		break;
	case SDLK_p:
		if (m_Character->m_gamestate == gamestate::start)
		{
			m_Character->SetGamestate(gamestate::playing);
		}
		break;
	case SDLK_r:
		if (m_Character->m_gamestate == gamestate::died)
		{
			m_Character->SetGamestate(gamestate::newlvl);
		}
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_a:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_d:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_w:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_s:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_RIGHT:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_LEFT:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_UP:
		m_Character->SetDirection(Directions::stopped);
		break;
	case SDLK_DOWN:
		m_Character->SetDirection(Directions::stopped);
		break;
	default:
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
