#pragma once
#include <utils.h>
#include <vector>
#include <Texture.h>
#include <iostream>
using namespace utils;
enum Directions
{
	left,
	right,
	up,
	down,
	leftup,
	leftdown,
	rightup,
	rightdown,
	stopped
};
enum DirectionsSquare
{
	Sleft,
	Sright,
	Sup,
	Sdown,
	Supleft,
	Supright,
	Sdownleft,
	Sdownright,
	Sstopped
};
enum gamestate
{
	start,
	playing,
	newlvl,
	died
};
class Character
{
public:
	Character(Rectf viewport);
	~Character();

	void Update(float elapsedSec);
	void Draw();
	//void DrawBoard();
	void SetDirection(Directions direction);
	void RemoveDirection(Directions direction);
	void SetGamestate(gamestate spelstatus);
	gamestate		m_gamestate{ start };



private:
	//void MakeBoard();
	void ChekOutOfBounds();
	void CollisionWithPrinces();
	void CollisionWithSquares();
	// updating the collision box
	void UpdateCollisionPoints();
	void Makelvl();
	void ResetAll();



	Rectf				m_ViewPort{};
	Point2f				m_location{ Point2f{225,225} };
	Point2f				m_LocationPrinces{ Point2f{0,0} };
	Rectf				m_CollisionBox{ Rectf{m_location.x,m_location.y,50,50} };
	Rectf				m_CollisionBoxPrinces{ m_LocationPrinces.x,m_LocationPrinces.y,40,40 };
	Rectf				m_Character{ Rectf{0,0,25,25} };
	Point2f				m_SquareMovingToYou{ Point2f{} };
	Point2f				m_GreenSquare{ Point2f{} };
	Point2f				m_Coin{ Point2f{} };
	Texture*			m_startTexture{};
	Texture*			m_diedTexture{};
	Texture*			m_scoreTexture{};
	Texture*			m_timeTexture{};
	Texture*			m_livesTexture{};
	Texture*			m_rtorestartTexture{};
	Texture*			m_highscoreTexture{};
	TTF_Font*			m_Font;
	float				m_width;
	int					m_highscore{ 0 };
	float				m_height;
	float				m_cellSize{ 10 };
	float				m_elapsedSeconds{0};
	float				random3{ 0 };
	float				m_timecalculator{0};
	int 				m_time{ 0 };
	int					m_score{ 0 };
	float				m_charactrspeed{ 5 };
	int					m_lives{ 3 };
	Directions			m_Direction{ Directions::stopped };
	DirectionsSquare	m_Directionssquare{ DirectionsSquare::Sright };
	DirectionsSquare	m_GreenSquareDirection{ DirectionsSquare::Sright };
	gamestate			m_previousegamestate{ start };
	//std::vector<std::vector<Rectf>> m_VectorOfSquares{};
	std::vector<Point2f> m_squars{};
	std::vector<DirectionsSquare> m_squarsDirections{};
	std::vector<float>	 m_SquaresSize{ 0 };
	std::vector<float>	 m_SquaresSpeed{ 0 };
	float				m_amountofsquares{1};
	float				m_squareSpeed{1};


	// out of bound bools
	bool				m_LeftOffScreen{ false };
	bool				m_RightOffScreen{ false };
	bool				m_BottomOffScreen{ false };
	bool				m_TopOffScreen{ false };

	bool				m_UpdateGreen{ false };
	bool				m_UpdateCoin{ false };
	bool				m_UpdateMovingSquare{ false };
};

