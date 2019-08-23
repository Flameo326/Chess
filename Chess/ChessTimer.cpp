#include "ChessTimer.h"
#include <iostream>
#include <chrono>

ChessTimer::ChessTimer()
{
	m_isStopped = true;
	m_isPlayer1Turn = true;
	m_gameOver = false;
	player1Time = 0.0f;
	player2Time = 0.0f;

	thread = new std::thread([this]() {
		while (!m_gameOver)
		{
			if (m_isStopped) 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			else
			{
				if (m_isPlayer1Turn)
				{
					player1Time += .01f;
				}
				else
				{
					player2Time += .01f;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}		
		}
	});
}

ChessTimer::~ChessTimer()
{
	m_isStopped = false;
	thread->join();
	delete thread;
}

void ChessTimer::start()
{
	m_isStopped = false;
}

void ChessTimer::stop()
{
	m_isStopped = true;
}

void ChessTimer::setPlayerTurn(bool value)
{
	m_isPlayer1Turn = value;
}

//void ChessTimer::changePlayer()
//{
//	m_isPlayer1Turn = !m_isPlayer1Turn;
//}

void ChessTimer::gameOver()
{
	m_gameOver = true;
}
