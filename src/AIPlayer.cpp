#include "AIPlayer.h"
#include <limits>
#include <memory>
#include <cmath>
#include <random>
#include "ConnectFour.h"
#include "TicTacToe.h"
#include "Theme.h"

namespace
{
	//0 = Very Easy, 1 = Easy, 2 = Medium, 3 = Hard, 4 = Very Hard
	static int g_aiDifficultyIndex = 2; // default to Medium
	// Theme index:0=Classic,1=Nature,2=Strawberry,3=Beachy,4=Dark
	static ThemeIndex g_themeIndex = ThemeIndex::Classic; // default to Classic

	double difficultyFactor()
	{
		switch (g_aiDifficultyIndex)
		{
		case 0: return 0.30; // Very Easy
		case 1: return 0.45; // Easy
		case 2: return 0.65; // Medium
		case 3: return 0.80; // Hard
		case 4: // fallthrough
		default: return 1.0; // Very Hard
		}
	}

	bool isBoardEmpty(const Game& game)
	{
		if (auto c4 = dynamic_cast<const ConnectFour*>(&game))
		{
			for (int row = 0; row < ConnectFour::HEIGHT; ++row)
			{
				for (int col = 0; col < ConnectFour::WIDTH; ++col)
				{
					if (c4->getCell(row, col) != Player::None)
						return false;
				}
			}
			return true;
		}

		if (auto ttt = dynamic_cast<const TicTacToe*>(&game))
		{
			for (int i = 0; i < 9; ++i)
			{
				if (ttt->getCell(i) != Player::None)
					return false;
			}
			return true;
		}

		return false;
	}

	Game::Move choosePreferredMove(const Game& game, const std::vector<Game::Move>& bestMoves)
	{
		if (auto c4 = dynamic_cast<const ConnectFour*>(&game))
		{
			const int centerPreference[] = {3, 2, 4, 1, 5, 0, 6};
			for (int col : centerPreference)
			{
				for (auto move : bestMoves)
				{
					if (move == col)
						return move;
				}
			}
		}

		if (auto ttt = dynamic_cast<const TicTacToe*>(&game))
		{
			const int priorityOrder[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
			for (int pos : priorityOrder)
			{
				for (auto move : bestMoves)
				{
					if (move == pos)
						return move;
				}
			}
		}

		return bestMoves.front();
	}
}

// Setter used by the UI to change difficulty at runtime
extern "C" void setAIDifficultyIndex(int idx)
{
	if (idx < 0) idx = 0;
	if (idx > 4) idx = 4;
	g_aiDifficultyIndex = idx;
}

extern "C" int getAIDifficultyIndex()
{
	return g_aiDifficultyIndex;
}

// Theme setter/getter (used at startup to apply persisted theme before views are created)
extern "C" void setThemeIndex(int idx)
{
	g_themeIndex = clampThemeIndex(idx);
}

extern "C" int getThemeIndex()
{
	return to_int(g_themeIndex);
}

int evaluateTerminal(const Game& state, Player aiPlayer, int depthRemaining)
{
	const Player winner = state.getWinner();
	if (winner == aiPlayer)
		return 1000 + depthRemaining; // prefer faster wins
	if (winner != Player::None)
		return -1000 - depthRemaining; // prefer slower losses
	if (state.isDraw())
		return 0;
	return 0;
}

int minimax(std::unique_ptr<Game> node, int depth, bool maximizing, int alpha, int beta, Player aiPlayer)
{
	if (depth == 0 || node->isGameOver())
		return evaluateTerminal(*node, aiPlayer, depth);

	const auto moves = node->getValidMoves();
	if (moves.empty())
		return evaluateTerminal(*node, aiPlayer, depth);

	if (maximizing)
	{
		int best = std::numeric_limits<int>::min();
		for (auto move : moves)
		{
			auto next = node->clone();
			if (!next->makeMove(move))
				continue;

			const int score = minimax(std::move(next), depth - 1, false, alpha, beta, aiPlayer);
			if (score > best)
				best = score;

			if (score > alpha)
				alpha = score;

			if (beta <= alpha)
				break;
		}
		return best;
	}
	else
	{
		int best = std::numeric_limits<int>::max();
		for (auto move : moves)
		{
			auto next = node->clone();
			if (!next->makeMove(move))
				continue;

			const int score = minimax(std::move(next), depth - 1, true, alpha, beta, aiPlayer);
			if (score < best)
				best = score;

			if (score < beta)
				beta = score;

			if (beta <= alpha)
				break;
		}
		return best;
	}
}

Game::Move AIPlayer::chooseMove(const Game& game, int maxDepth)
{
	if (maxDepth < 1)
		maxDepth = 1;

	// Compute effective search depth based on difficulty
	const double factor = difficultyFactor();
	int effectiveDepth = static_cast<int>(std::round(maxDepth * factor));
	if (effectiveDepth < 1)
		effectiveDepth = 1;

	// The existing logic calls minimax with (depth -1) after making the candidate move
	const int minimaxDepth = std::max(1, effectiveDepth);

	const Player aiPlayer = game.getCurrentPlayer();
	const auto moves = game.getValidMoves();
	if (moves.empty())
		return -1;

	int bestScore = std::numeric_limits<int>::min();
	std::vector<Game::Move> bestMoves;
	int alpha = std::numeric_limits<int>::min();
	int beta = std::numeric_limits<int>::max();

	for (auto move : moves)
	{
		auto next = game.clone();
		if (!next->makeMove(move))
			continue;

		const int score = minimax(std::move(next), minimaxDepth - 1, false, alpha, beta, aiPlayer);
		if (score > bestScore)
		{
			bestScore = score;
			bestMoves.clear();
			bestMoves.push_back(move);
		}
		else if (score == bestScore)
		{
			bestMoves.push_back(move);
		}

		if (score > alpha)
			alpha = score;
	}

	if (bestMoves.empty())
		return moves.front();

	// Randomize only the opening move for variety, then stay deterministic
	if (bestMoves.size() > 1 && isBoardEmpty(game))
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, static_cast<int>(bestMoves.size()) - 1);
		return bestMoves[dis(gen)];
	}

	return choosePreferredMove(game, bestMoves);
}