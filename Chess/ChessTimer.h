#pragma once
#include <thread>
#include <functional>

class ChessTimer
{
public:
	ChessTimer();
	~ChessTimer();
	void start();
	void stop();
	void changePlayer();
	void gameOver();

	float getPlayer1Time() { return player1Time; }

	float getPlayer2Time() { return player2Time; }

private:

	bool m_isStopped;
	bool m_isPlayer1Turn;
	bool m_gameOver;
	float player1Time;
	float player2Time;
	std::thread * thread;
};

