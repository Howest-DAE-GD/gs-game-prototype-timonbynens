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
				m_squars[index].x -= m_squareSpeed;
				break;
			case Sright:
				m_squars[index].x += m_squareSpeed;
				break;
			case Sup:
				m_squars[index].y += m_squareSpeed;
				break;
			case Sdown:
				m_squars[index].y -= m_squareSpeed;
				break;
			case Supleft:
				m_squars[index].x -= m_squareSpeed;
				m_squars[index].y += m_squareSpeed;
				break;
			case Supright:
				m_squars[index].x += m_squareSpeed;
				m_squars[index].y += m_squareSpeed;
				break;
			case Sdownleft:
				m_squars[index].x -= m_squareSpeed;
				m_squars[index].y -= m_squareSpeed;
				break;
			case Sdownright:
				m_squars[index].x += m_squareSpeed;
				m_squars[index].y -= m_squareSpeed;
				break;
			case Sstopped:
				break;
			default:
				break;
			}
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
			FillRect(m_squars[index].x, m_squars[index].y, m_Character.width + 5, m_Character.height + 5);
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

}
void Character::CollisionWithPrinces()
{
	if (IsOverlapping(m_CollisionBox, m_CollisionBoxPrinces))
	{
		m_gamestate = gamestate::newlvl;
		if (m_amountofsquares < 8)
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
		if (IsOverlapping(m_CollisionBox, Rectf{ m_squars[index].x, m_squars[index].y, m_Character.width + 5, m_Character.height + 5}) && m_elapsedSeconds > 1)
		{
			m_lives--;
			m_charactrspeed--;
			m_location = Point2f{ 225,225 };
			m_elapsedSeconds = 0;
		}
		if (m_lives == 0)
		{
			m_gamestate = gamestate::died;
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
	utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
	FillRect(10,10,10,10);
	int random = rand() % 3 + 1;
	if (random == 1)
	{
		m_LocationPrinces.x = 0;
		m_LocationPrinces.y = 0;
	}
	else if (random == 2)
	{
		m_LocationPrinces.x = m_ViewPort.width -m_CollisionBoxPrinces.width;
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
	if (m_amountofsquares > 3)
	{
		m_squareSpeed += 0.5f;
	}
	for (int index = 0; index < m_amountofsquares; index++)
	{
		float random1 = rand() % int(m_ViewPort.width);
		float random2 = rand() % int(m_ViewPort.height);
		m_squars.push_back(Point2f{ random1, random2 });
		random3 = rand() % 7;
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
}
