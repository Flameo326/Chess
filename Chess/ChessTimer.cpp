#include "ChessTimer.h"
#include <chrono>

ChessTimer::ChessTimer()
{
	m_isStopped = false;
	m_isPlayer1Turn = true;
	thread = new std::thread([this]() {
		while (m_gameOver)
		{
			if (m_isStopped) std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			if (m_isPlayer1Turn)
			{
				player1Time += .001f;
			}
			else
			{
				player2Time += .001f;
			}
			if (m_isStopped) std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

void ChessTimer::changePlayer()
{
	m_isPlayer1Turn = !m_isPlayer1Turn;
}

void ChessTimer::gameOver()
{
	m_gameOver = true;
}
