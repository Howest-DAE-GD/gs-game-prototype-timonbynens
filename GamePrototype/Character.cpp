#include "pch.h"
#include "Character.h"

Character::Character(Rectf viewport)
{
	if (TTF_Init() == -1)
	{
		std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
		exit(1);
	}

	m_Font = TTF_OpenFont("arial.ttf", 24);
	if (!m_Font)
	{
		std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(1);
	}
	m_ViewPort = viewport;
	m_width = 10;
	m_height = 10;
	m_cellSize = viewport.width / 10;
	Makelvl();
	m_startTexture = new Texture("Press P to start", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_diedTexture = new Texture("U died", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_scoreTexture = new Texture("0", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_timeTexture = new Texture("0", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_livesTexture = new Texture("3", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_rtorestartTexture = new Texture("Press R to restart", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	m_highscoreTexture = new Texture("Highscore", "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
}

Character::~Character()
{
	delete m_startTexture;
	delete m_diedTexture;
	delete m_scoreTexture;
	delete m_timeTexture;
	delete m_livesTexture;
	delete m_rtorestartTexture;
}

void Character::Update(float elapsedSec)
{
	if (m_previousegamestate == gamestate::died && m_gamestate == gamestate::newlvl)
	{
		ResetAll();
	}
	if (m_gamestate == gamestate::start)
	{
		std::string starttext = "Press P to start";
		delete m_startTexture;
		m_startTexture = new Texture(starttext, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		m_previousegamestate = m_gamestate;
	}
	else if (m_gamestate == gamestate::playing)
	{
		if (m_UpdateMovingSquare == true)
		{
			if (m_SquareMovingToYou.y - m_location.y > 0)
			{
				m_SquareMovingToYou.y -= 1.2;
			}
			else
			{
				m_SquareMovingToYou.y += 1.2;
			}
			if (m_SquareMovingToYou.x - m_location.x > 0)
			{
				m_SquareMovingToYou.x -= 1.2;
			}
			else
			{
				m_SquareMovingToYou.x += 1.2;
			}
		}
		m_timecalculator += elapsedSec;
		m_elapsedSeconds += elapsedSec;
		UpdateCollisionPoints();
		CollisionWithPrinces();
		CollisionWithSquares();
		ChekOutOfBounds();
		switch (m_Direction)
		{
		case Directions::left:
			if (!m_LeftOffScreen)
			{
				m_location.x -= m_charactrspeed;
			}
			break;
		case Directions::right:
			if (!m_RightOffScreen)
			{
				m_location.x += m_charactrspeed;
			}
			break;
		case Directions::up:
			if (!m_TopOffScreen)
			{
				m_location.y += m_charactrspeed;
			}
			break;
		case Directions::down:
			if (!m_BottomOffScreen)
			{
				m_location.y -= m_charactrspeed;
			}
			break;
		case Directions::leftup:
			if (!m_LeftOffScreen)
			{
				m_location.x -= m_charactrspeed / 2 + 1;
			}		
			if (!m_TopOffScreen)
			{
				m_location.y += m_charactrspeed / 2 + 1;
			}
			break;								    
		case Directions::rightup:				    
			if (!m_RightOffScreen)
			{
				m_location.x += m_charactrspeed / 2 + 1;
			}	
			if (!m_TopOffScreen)
			{
				m_location.y += m_charactrspeed / 2 + 1;
			}
			break;								   
		case Directions::leftdown:				   
			if (!m_LeftOffScreen)
			{
				m_location.x -= m_charactrspeed / 2 + 1;
			}	
			if (!m_BottomOffScreen)
			{
				m_location.y -= m_charactrspeed / 2 + 1;
			}
			break;								 
		case Directions::rightdown:				 
			if (!m_RightOffScreen)				 
			{
				m_location.x += m_charactrspeed / 2 + 1;
			}
			if (!m_BottomOffScreen)
			{
				m_location.y -= m_charactrspeed / 2 + 1;
			}
			break;
		default:
			break;
		}

		m_Character.left = m_location.x;
		m_Character.bottom = m_location.y;
		for (int index = 0; index < m_squars.size(); index++)
		{
			switch (m_squarsDirections[index])
			{
			case Sleft:
				m_squars[index].x -= m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sright:
				m_squars[index].x += m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sup:
				m_squars[index].y += m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sdown:
				m_squars[index].y -= m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Supleft:
				m_squars[index].x -= m_squareSpeed + m_SquaresSpeed[index];
				m_squars[index].y += m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Supright:
				m_squars[index].x += m_squareSpeed + m_SquaresSpeed[index];
				m_squars[index].y += m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sdownleft:
				m_squars[index].x -= m_squareSpeed + m_SquaresSpeed[index];
				m_squars[index].y -= m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sdownright:
				m_squars[index].x += m_squareSpeed + m_SquaresSpeed[index];
				m_squars[index].y -= m_squareSpeed + m_SquaresSpeed[index];
				break;
			case Sstopped:
				break;
			default:
				break;
			}
		}
		if (m_UpdateGreen == true)
		{
			switch (m_GreenSquareDirection)
			{
			case Sleft:
				m_GreenSquare.x -= m_squareSpeed;
				break;
			case Sright:
				m_GreenSquare.x += m_squareSpeed;
				break;
			case Sup:
				m_GreenSquare.y += m_squareSpeed;
				break;
			case Sdown:
				m_GreenSquare.y -= m_squareSpeed;
				break;
			case Supleft:
				m_GreenSquare.x -= m_squareSpeed;
				m_GreenSquare.y += m_squareSpeed;
				break;
			case Supright:
				m_GreenSquare.x += m_squareSpeed;
				m_GreenSquare.y += m_squareSpeed;
				break;
			case Sdownleft:
				m_GreenSquare.x -= m_squareSpeed;
				m_GreenSquare.y -= m_squareSpeed;
				break;
			case Sdownright:
				m_GreenSquare.x += m_squareSpeed;
				m_GreenSquare.y -= m_squareSpeed;
				break;
			case Sstopped:
				break;
			default:
				break;
			}
		}
		if (m_UpdateCoin == true)
		{

		}
			
		std::string starttext = "Time: " + std::to_string(m_time);
		delete m_scoreTexture;
		m_scoreTexture = new Texture(starttext, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		std::string starttext2 = "Score: " + std::to_string(m_score);
		delete m_timeTexture;
		m_timeTexture = new Texture(starttext2, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		std::string starttext3 = "Lives " + std::to_string(m_lives);
		delete m_livesTexture;
		m_livesTexture = new Texture(starttext3, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		m_previousegamestate = m_gamestate;
		
	}
	else if (m_gamestate == gamestate::newlvl)
	{
		m_elapsedSeconds = 0;
		Makelvl();
		m_previousegamestate = m_gamestate;
	}
	else if (m_gamestate == gamestate::died)
	{
		std::string starttext3 = "Highscore:" + std::to_string(m_highscore);
		delete m_highscoreTexture;
		m_highscoreTexture = new Texture(starttext3, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		std::string starttext = "You died";
		delete m_diedTexture;
		m_diedTexture = new Texture(starttext, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		std::string starttext2 = "Press R to restart";
		delete m_rtorestartTexture;
		m_rtorestartTexture = new Texture(starttext2, "arial.ttf", 24, Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		m_previousegamestate = m_gamestate;
	}
	m_time = m_timecalculator;
}

void Character::Draw()
{
	if (m_gamestate == gamestate::start)
	{
		m_startTexture->Draw(Point2f{ 175, 255 });
	}
	else if (m_gamestate == gamestate::playing)
	{
		utils::SetColor(Color4f(1.f, 0.f, 0.0f, 1.f));
		FillRect(m_Character);
		utils::SetColor(Color4f(1.f, 1.f, 0.0f, 1.f));
		for (int index = 0; index < m_squars.size(); index++)
		{
			FillRect(m_squars[index].x, m_squars[index].y, m_Character.width + 5 + m_SquaresSize[index], m_Character.height + 5 + m_SquaresSize[index]);
		}
		if (m_UpdateMovingSquare == true)
		{
			utils::SetColor(Color4f(1.f, 0.5f, 0.1f, 1.f));
			FillRect(m_SquareMovingToYou.x, m_SquareMovingToYou.y, m_Character.width + 5, m_Character.height + 5);
		}
		if (m_UpdateGreen == true)
		{
			utils::SetColor(Color4f(0.004f, 0.6f, 0.080f, 1.f));
			FillRect(m_GreenSquare.x, m_GreenSquare.y, m_Character.width + 5, m_Character.height + 5);
		}
		if (m_UpdateCoin == true)
		{
			utils::SetColor(Color4f(0.6f, 0.008f, 0.080f, 1.f));
			FillRect(m_Coin.x, m_Coin.y, m_Character.width + 5, m_Character.height + 5);
		}
		utils::SetColor(Color4f(0.f, 1.f, 1.0f, 1.f));
		FillRect(m_CollisionBoxPrinces);
		m_timeTexture->Draw(Point2f{ m_ViewPort.width - 160, m_ViewPort.height - 30 });
		m_scoreTexture->Draw(Point2f{ 60, m_ViewPort.height - 30 });
		m_livesTexture->Draw(Point2f{ m_ViewPort.width/2 - 20, m_ViewPort.height - 30 });
	}
	else if (m_gamestate == gamestate::newlvl)
	{

	}
	else if (m_gamestate == gamestate::died)
	{
		m_highscoreTexture->Draw(Point2f{ 190, 345 });
		m_timeTexture->Draw(Point2f{ 155, 300 });
		m_scoreTexture->Draw(Point2f{ 265, 300 });
		m_diedTexture->Draw(Point2f{ 212, 255 });
		m_rtorestartTexture->Draw(Point2f(160, 215));
	}

}

//void Character::DrawBoard()
//{
//	for (int index = 0; index < m_VectorOfSquares.size(); index++)
//	{
//		for (int index2 = 0; index2  < m_VectorOfSquares[index].size(); index2++)
//		{
//			if (index % 2 == 0)
//			{
//				if (index2 % 2 == 0)
//				{
//					utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
//					FillRect(m_VectorOfSquares[index][index2]);
//				}
//				else
//				{
//					utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
//					FillRect(m_VectorOfSquares[index][index2]);
//				}
//			}
//			else if (index2 % 2 != 0)
//			{
//				utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
//				FillRect(m_VectorOfSquares[index][index2]);
//			}
//			else
//			{
//				utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
//				FillRect(m_VectorOfSquares[index][index2]);
//			}
//		}
//		
//	}
//}

void Character::SetDirection(Directions direction)
{
	m_Direction = direction;
}

void Character::RemoveDirection(Directions direction)
{
	if (m_Direction == Directions::left && direction == Directions::left)
	{
		m_Direction = Directions::stopped;
	}
	else if (m_Direction == Directions::right && direction == Directions::right)
	{
		m_Direction = Directions::stopped;
	}
	else if (m_Direction == Directions::up && direction == Directions::up)
	{
		m_Direction = Directions::stopped;
	}
	else if (m_Direction == Directions::down && direction == Directions::down)
	{
		m_Direction = Directions::stopped;
	}
	else if (m_Direction == Directions::leftup)
	{
		if (direction == Directions::left)
		{
			m_Direction = Directions::up;
		}
		else if (direction == Directions::up)
		{
			m_Direction = Directions::left;
		}
	}
	else if (m_Direction == Directions::leftdown)
	{
		if (direction == Directions::left)
		{
			m_Direction = Directions::down;
		}
		else if (direction == Directions::down)
		{
			m_Direction = Directions::left;
		}
	}
	else if (m_Direction == Directions::rightup)
	{
		if (direction == Directions::right)
		{
			m_Direction = Directions::up;
		}
		else if (direction == Directions::up)
		{
			m_Direction = Directions::right;
		}
	}
	else if (m_Direction == Directions::rightdown)
	{
		if (direction == Directions::down)
		{
			m_Direction = Directions::right;
		}
		else if (direction == Directions::right)
		{
			m_Direction = Directions::down;
		}
	}
	else
	{
		m_Direction = Directions::stopped;
	}
}

void Character::SetGamestate(gamestate spelstatus)
{
	m_gamestate = spelstatus;
	
}

//void Character::MakeBoard()
//{
//	for (int x = 0; x < m_width; ++x)
//	{
//		std::vector<Rectf> vector;
//		for (int y = 0; y < m_height; ++y)
//		{
//			if (x % 2 == 0)
//			{
//				if (y % 2 == 0)
//				{
//					vector.push_back(Rectf{x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize});
//				}
//				else
//				{
//					vector.push_back(Rectf{ x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize });
//				}
//			}
//			else if (y % 2 != 0)
//			{
//				vector.push_back(Rectf{ x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize });
//			}
//			else
//			{
//				vector.push_back(Rectf{ x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize });
//			}
//		}
//		m_VectorOfSquares.push_back(vector);
//		vector.clear();
//	}
//}

void Character::ChekOutOfBounds()
{

	if (m_CollisionBox.bottom <= 0)
	{
		m_BottomOffScreen = true;
	}
	else
	{
		m_BottomOffScreen = false;
	}
	if (m_CollisionBox.bottom + m_Character.height >= m_ViewPort.height )
	{
		m_TopOffScreen = true;
	}
	else
	{
		m_TopOffScreen = false;
	}
	if (m_CollisionBox.left <= 0)
	{
		m_LeftOffScreen = true;
	}
	else
	{
		m_LeftOffScreen = false;
	}
	if (m_CollisionBox.left + m_Character.width >= m_ViewPort.width)
	{
		m_RightOffScreen = true;
	}
	else
	{
		m_RightOffScreen = false;
	}
	for (int index = 0; index < m_squars.size(); index++)
	{
		if (m_squars[index].x <= 0)
		{
			if (m_squarsDirections[index] == DirectionsSquare::Supleft)
			{
m_squarsDirections[index] = DirectionsSquare::Supright;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sdownleft)
			{
				m_squarsDirections[index] = DirectionsSquare::Sdownright;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sleft)
			{
				m_squarsDirections[index] = DirectionsSquare::Sright;
			}
		}
		if (m_squars[index].x + m_Character.width + 5 >= m_ViewPort.width)
		{
			if (m_squarsDirections[index] == DirectionsSquare::Supright)
			{
				m_squarsDirections[index] = DirectionsSquare::Supleft;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sdownright)
			{
				m_squarsDirections[index] = DirectionsSquare::Sdownleft;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sright)
			{
				m_squarsDirections[index] = DirectionsSquare::Sleft;
			}
		}
		if (m_squars[index].y <= 0)
		{
			if (m_squarsDirections[index] == DirectionsSquare::Sdownright)
			{
				m_squarsDirections[index] = DirectionsSquare::Supright;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sdownleft)
			{
				m_squarsDirections[index] = DirectionsSquare::Supleft;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sdown)
			{
				m_squarsDirections[index] = DirectionsSquare::Sup;
			}
		}
		if (m_squars[index].y + m_Character.height + 5 >= m_ViewPort.height)
		{
			if (m_squarsDirections[index] == DirectionsSquare::Supright)
			{
				m_squarsDirections[index] = DirectionsSquare::Sdownright;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Supleft)
			{
				m_squarsDirections[index] = DirectionsSquare::Sdownleft;
			}
			else if (m_squarsDirections[index] == DirectionsSquare::Sup)
			{
				m_squarsDirections[index] = DirectionsSquare::Sdown;
			}
		}


	}
	if (m_GreenSquare.x <= 0)
	{
		if (m_GreenSquareDirection == DirectionsSquare::Supleft)
		{
			m_GreenSquareDirection = DirectionsSquare::Supright;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sdownleft)
		{
			m_GreenSquareDirection = DirectionsSquare::Sdownright;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sleft)
		{
			m_GreenSquareDirection = DirectionsSquare::Sright;
		}
	}
	if (m_GreenSquare.x + m_Character.width + 5 >= m_ViewPort.width)
	{
		if (m_GreenSquareDirection == DirectionsSquare::Supright)
		{
			m_GreenSquareDirection = DirectionsSquare::Supleft;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sdownright)
		{
			m_GreenSquareDirection = DirectionsSquare::Sdownleft;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sright)
		{
			m_GreenSquareDirection = DirectionsSquare::Sleft;
		}
	}
	if (m_GreenSquare.y <= 0)
	{
		if (m_GreenSquareDirection == DirectionsSquare::Sdownright)
		{
			m_GreenSquareDirection = DirectionsSquare::Supright;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sdownleft)
		{
			m_GreenSquareDirection = DirectionsSquare::Supleft;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sdown)
		{
			m_GreenSquareDirection = DirectionsSquare::Sup;
		}
	}
	if (m_GreenSquare.y + m_Character.height + 5 >= m_ViewPort.height)
	{
		if (m_GreenSquareDirection == DirectionsSquare::Supright)
		{
			m_GreenSquareDirection = DirectionsSquare::Sdownright;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Supleft)
		{
			m_GreenSquareDirection = DirectionsSquare::Sdownleft;
		}
		else if (m_GreenSquareDirection == DirectionsSquare::Sup)
		{
			m_GreenSquareDirection = DirectionsSquare::Sdown;
		}
	}
}
void Character::CollisionWithPrinces()
{
	if (IsOverlapping(m_CollisionBox, m_CollisionBoxPrinces))
	{
		m_gamestate = gamestate::newlvl;
		if (m_amountofsquares < 6)
		{
			m_amountofsquares++;
		}
		m_score++;
		if (m_score > m_highscore)
		{
			m_highscore = m_score;
		}
	}
}
void Character::CollisionWithSquares()
{
	for (int index = 0; index < m_squars.size(); index++)
	{
		if (IsOverlapping(m_CollisionBox, Rectf{ m_squars[index].x, m_squars[index].y, m_Character.width + 5 + m_SquaresSize[index], m_Character.height + 5 + m_SquaresSize[index] }) && m_elapsedSeconds > 1)
		{
			m_lives--;
			m_charactrspeed--;
			m_location = Point2f{ 225,225 };
			m_elapsedSeconds = 0;
		}
	}
	if (m_UpdateMovingSquare == true)
	{
		if (IsOverlapping(m_CollisionBox, Rectf{ m_SquareMovingToYou.x, m_SquareMovingToYou.y, m_Character.width + 5, m_Character.height + 5 }) && m_elapsedSeconds > 1)
		{
			m_lives--;
			m_charactrspeed--;
			m_location = Point2f{ 225,225 };
			m_UpdateMovingSquare = false;
			m_elapsedSeconds = 0;
		}
	}
	if(m_UpdateGreen == true)
	{
		if (IsOverlapping(m_CollisionBox, Rectf{ m_GreenSquare.x, m_GreenSquare.y, m_Character.width + 5, m_Character.height + 5 }) && m_elapsedSeconds > 1)
		{
			m_lives++;
			m_charactrspeed++;
			m_UpdateGreen = false;
		}
	}
	if (m_lives == 0)
	{
		m_gamestate = gamestate::died;
	}
	if (m_UpdateCoin == true)
	{
		if (IsOverlapping(m_CollisionBox, Rectf{ m_Coin.x, m_Coin.y, m_Character.width + 5, m_Character.height + 5 }) && m_elapsedSeconds > 1)
		{
			m_score++;
			m_UpdateCoin = false;
		}
	}

}
void Character::UpdateCollisionPoints()
{
	// default collision box
	m_CollisionBox.bottom = m_location.y;
	m_CollisionBox.left = m_location.x;
	m_CollisionBox.width = m_Character.width;
	m_CollisionBox.height = m_Character.height;
	m_CollisionBoxPrinces.left = m_LocationPrinces.x;
	m_CollisionBoxPrinces.bottom = m_LocationPrinces.y;
}

void Character::Makelvl()
{
	m_location = Point2f{ 225,225 };
	m_squars.clear();
	m_squarsDirections.clear();
	m_SquaresSize.clear();
	m_SquaresSpeed.clear();
	utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
	FillRect(10, 10, 10, 10);
	int random = rand() % 3 + 1;
	if (random == 1)
	{
		m_LocationPrinces.x = 0;
		m_LocationPrinces.y = 0;
	}
	else if (random == 2)
	{
		m_LocationPrinces.x = m_ViewPort.width - m_CollisionBoxPrinces.width;
		m_LocationPrinces.y = 0;
	}
	else if (random == 3)
	{
		m_LocationPrinces.x = 0;
		m_LocationPrinces.y = m_ViewPort.height - m_CollisionBoxPrinces.height;
	}
	else if (random == 4)
	{
		m_LocationPrinces.x = m_ViewPort.width - m_CollisionBoxPrinces.width;
		m_LocationPrinces.y = m_ViewPort.height - m_CollisionBoxPrinces.height;;
	}
	if (m_amountofsquares > 2)
	{
		m_squareSpeed += 0.1f;
	}
	else
	{
		m_squareSpeed += 0.3f;
	}
	for (int index = 0; index < m_amountofsquares; index++)
	{
		float random1 = float{};
		float random2 = float{};
		if (m_score > 7)
		{
			float randomType = rand() % 8;
			if (randomType == 1)
			{
				m_SquaresSize.push_back(12);
				m_SquaresSpeed.push_back(-1);
			}
			else if (randomType == 2)
			{
				m_SquaresSize.push_back(-8);
				m_SquaresSpeed.push_back(0.5);
			}
			else
			{
				m_SquaresSize.push_back(0);
				m_SquaresSpeed.push_back(0);
			}
		}
		else if (m_score > 9)
		{
			float randomType = rand() % 5;
			if (randomType == 1)
			{
				m_SquaresSize.push_back(12);
				m_SquaresSpeed.push_back(-1);
			}
			else if (randomType == 2)
			{
				m_SquaresSize.push_back(-8);
				m_SquaresSpeed.push_back(0.5);
			}
			else
			{
				m_SquaresSize.push_back(0);
				m_SquaresSpeed.push_back(0);
			}
		}
		else if (m_score > 11)
		{
			float randomType = rand() % 3;
			if (randomType == 1)
			{
				m_SquaresSize.push_back(12);
				m_SquaresSpeed.push_back(-1);
			}
			else if (randomType == 2)
			{
				m_SquaresSize.push_back(-8);
				m_SquaresSpeed.push_back(0.5);
			}
			else
			{
				m_SquaresSize.push_back(0);
				m_SquaresSpeed.push_back(0);
			}
		}
		else
		{
			m_SquaresSize.push_back(0);
			m_SquaresSpeed.push_back(0);
		}
		random1 = rand() % int(m_ViewPort.width - m_Character.width - 5);
		random2 = rand() % int(m_ViewPort.height - m_Character.height - 5);
		m_squars.push_back(Point2f{ random1, random2 });
		float random3 = rand() % 7;
		if (random3 == 0.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sleft);
		}
		else if (random3 == 1.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sright);
		}
		else if (random3 == 2.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sup);
		}
		else if (random3 == 3.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sdown);
		}
		else if (random3 == 4.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Supleft);
		}
		else if (random3 == 5.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Supright);
		}
		else if (random3 == 6.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sdownleft);
		}
		else if (random3 == 7.f)
		{
			m_squarsDirections.push_back(DirectionsSquare::Sdownright);
		}
		if (m_previousegamestate != gamestate::start)
		{
			m_gamestate = gamestate::playing;
		}
	}
	float random5 = rand() % 5;
	if (m_score > 11)
	{
		m_UpdateCoin = true;
		if (m_LocationPrinces.x > 250 && m_LocationPrinces.y < 250)
		{
			m_Coin = Point2f{ 100,400 };
		}
		else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y < 250)
		{
			m_Coin = Point2f{ 400,400 };
		}
		else if (m_LocationPrinces.x > 250 && m_LocationPrinces.y > 250)
		{
			m_Coin = Point2f{ 100,100 };
		}
		else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y > 250)
		{
			m_Coin = Point2f{ 400,100 };
		}
		if (m_amountofsquares > 5)
		{
			if (m_LocationPrinces.x > 250 && m_LocationPrinces.y < 250)
			{
				m_SquareMovingToYou = Point2f{ 100,400 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y < 250)
			{
				m_SquareMovingToYou = Point2f{ 400,400 };
			}
			else if (m_LocationPrinces.x > 250 && m_LocationPrinces.y > 250)
			{
				m_SquareMovingToYou = Point2f{ 100,100 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y > 250)
			{
				m_SquareMovingToYou = Point2f{ 400,100 };
			}
			m_UpdateMovingSquare = true;
		}
	}
	else if (random5 == 1)
	{
		m_UpdateCoin = true;
		m_UpdateMovingSquare = false;
		if (m_LocationPrinces.x > 250 && m_LocationPrinces.y < 250)
		{
			m_Coin = Point2f{ 100,400 };
		}
		else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y < 250)
		{
			m_Coin = Point2f{ 400,400 };
		}
		else if (m_LocationPrinces.x > 250 && m_LocationPrinces.y > 250)
		{
			m_Coin = Point2f{ 100,100 };
		}
		else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y > 250)
		{
			m_Coin = Point2f{ 400,100 };
		}	
	}
	else
	{
		m_UpdateCoin = false;
		if (m_amountofsquares > 5)
		{
			if (m_LocationPrinces.x > 250 && m_LocationPrinces.y < 250)
			{
				m_SquareMovingToYou = Point2f{ 100,400 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y < 250)
			{
				m_SquareMovingToYou = Point2f{ 400,400 };
			}
			else if (m_LocationPrinces.x > 250 && m_LocationPrinces.y > 250)
			{
				m_SquareMovingToYou = Point2f{ 100,100 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y > 250)
			{
				m_SquareMovingToYou = Point2f{ 400,100 };
			}
			m_UpdateMovingSquare = true;
		}
	}
	if (m_lives < 3)
	{
		float random4 = rand() % 10;
		if (random4 == 1)
		{
			if (m_LocationPrinces.x > 250 && m_LocationPrinces.y < 250)
			{
				m_GreenSquare = Point2f{ 0,390 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y < 250)
			{
				m_GreenSquare = Point2f{ 290,490 };
			}
			else if (m_LocationPrinces.x > 250 && m_LocationPrinces.y > 250)
			{
				m_GreenSquare = Point2f{ 210,80 };
			}
			else if (m_LocationPrinces.x < 250 && m_LocationPrinces.y > 250)
			{
				m_GreenSquare = Point2f{ 90,0 };
			}
			m_UpdateGreen = true;
			m_GreenSquareDirection = DirectionsSquare::Sdownleft;
		}
		else
		{
			m_UpdateGreen = false;
		}
	}
}


void Character::ResetAll()
{
	m_location = Point2f{ 225,225 };
	m_amountofsquares = 1;
	m_squareSpeed = 1;
	m_time = 0;
	m_timecalculator = 0;
	m_score = 0;
	m_lives = 3;
	m_charactrspeed = 5;
	m_SquareMovingToYou = Point2f{0,0};
	m_GreenSquare = Point2f{ 0,0 };
	m_Coin = Point2f{ 0,0 };
	m_UpdateMovingSquare = false;
	m_UpdateCoin = false;
	m_UpdateGreen = false;
}
