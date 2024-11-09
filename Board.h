#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Spaces.h"
#include "Random.h"
#include "enum_.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

// CLASS FOR TILES
struct Tiles
{
	// manages the tiles/spaces and what's under them
	bool isMine = false;
	bool isHidden = true;
	bool hasFlag = false;
	int adjacentMines = 0;
	int row = 0;
	int col = 0;
	vector<Tiles*> adjacentTiles;

};

// CLASS FOR BOARD

struct Board
{
	int numCols;
	int numRows;
	int numMines;
	vector<vector<Tiles>> board; // spaces make up the board
	vector<vector<int>> mines; // goal of game is to avoid
	int flagCounter = 0;
	int minesRemaining;

	// private functions
	void InitializeBoard()
	{
		minesRemaining = numMines;
		cout << "Number of mines is: " << minesRemaining << endl;
		// make sure the board and mines vectors are properly sized
		board.resize(numRows, vector<Tiles>(numCols));
		mines.resize(numRows, vector<int>(numCols, 0));

		// initialize tiles on the board
		for (int i = 0; i < numRows; ++i) {
			for (int j = 0; j < numCols; ++j) {
				board[i][j].row = i;
				board[i][j].col = j;
			}
		}

		//SetMines();
		AdjacentMines();
	}

	void InitializeBoardWithMines(Board& board)
	{
		board.InitializeBoard(); // initialize board

		// place mines without revealing any tiles
		int minesPlaced = 0;
		while (minesPlaced < board.numMines)
		{
			int row = Random::Int(0, board.numRows - 1);
			int col = Random::Int(0, board.numCols - 1);

			if (!board.board[row][col].isMine)
			{
				board.board[row][col].isMine = true;
				minesPlaced++;
			}
		}

	}

	void SetMines()
	{
		// number of mines does not exceed the total number of tiles
		int maxMines = numCols * numRows;
		minesRemaining = (numMines <= maxMines) ? numMines : maxMines;

		while (minesRemaining > 0)
		{
			int randomRow = Random::Int(0, numRows - 1);
			int randomCol = Random::Int(0, numCols - 1);

			// check if the tile doesn't already have a mine
			if (!board[randomRow][randomCol].isMine)
			{
				board[randomRow][randomCol].isMine = true;
				minesRemaining--;
			}
		}
	}

	void AdjacentMines()
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; ++col)
			{
				Tiles& currentTile = board[row][col];

				currentTile.adjacentTiles.clear();
				currentTile.adjacentMines = 0; // Reset the count

				for (int i = -1; i <= 1; ++i)
				{
					for (int j = -1; j <= 1; ++j)
					{
						int neighborRow = row + i;
						int neighborCol = col + j;

						if (neighborRow >= 0 && neighborRow < numRows && neighborCol >= 0 && neighborCol < numCols)
						{
							if (!(i == 0 && j == 0))
							{
								// pointer to adjacent tile
								currentTile.adjacentTiles.push_back(&board[neighborRow][neighborCol]);

								// Count adjacent mines
								if (board[neighborRow][neighborCol].isMine)
								{
									currentTile.adjacentMines++;
								}
							}
						}
					}
				}
			}
		}
	}


	//Board(int numRows, int numCols, int numMines);
	Board() {}

	Board(int columns, int rows, int mines)
	{
		numCols = columns;
		numRows = rows;
		numMines = mines;

		vector<vector<Tiles>> board(numRows, vector<Tiles>(numCols));
		for (auto i = 0; i < numRows; i++) {
			for (auto j = 0; j < numCols; j++)
			{
				board[i][j].row = i;
				board[i][j].col = j;
			}
		}
		this->board = board;
	}
	void Draw(sf::RenderWindow& window)
	{

	}

	void RestartBoard()
	{
		// reset all tiles to its initial state
		for (int i = 0; i < numRows; ++i)
		{
			for (int j = 0; j < numCols; ++j)
			{
				board[i][j].isMine = false;
				board[i][j].isHidden = true;
				board[i][j].hasFlag = false;
				board[i][j].adjacentMines = 0;
			}
		}

		SetMines();
		AdjacentMines();

		flagCounter = 0;
		minesRemaining = numMines;  // set minesRemaining to the current number of mines to adhere to different files

		mineClicked = false;
		gameEnded = false;
		gameState = GameState::PLAYING;
	}

	void SetFlag(int col, int row)
	{
		if (board[row][col].isHidden)
		{
			if (!board[row][col].hasFlag && minesRemaining > 0)
			{
				// place a flag
				board[row][col].hasFlag = true;
				minesRemaining--;
			}
			else if (board[row][col].hasFlag)
			{
				// remove the flag
				board[row][col].hasFlag = false;
				minesRemaining++;
			}
		}

		// Update adjacent mines after flag changes
		AdjacentMines();
	}

	bool debugMode = false;
	bool mineClicked = false;
	bool gameEnded = false;

	void RevealSpace(int col, int row)
	{
		if (col < 0 || col >= numCols || row < 0 || row >= numRows || !board[row][col].isHidden || board[row][col].hasFlag)
		{
			return;
		}

		// reveal the current space
		board[row][col].isHidden = false;

		// If the current space is empty, reveal adjacent spaces
		if (board[row][col].adjacentMines == 0)
		{
			RevealSpace(col - 1, row); // left
			RevealSpace(col + 1, row); // right
			RevealSpace(col, row - 1); // up
			RevealSpace(col, row + 1); // down
		}

		// check if is mine
		if (board[row][col].isMine)
		{
			mineClicked = true;
		}
	}


	void RevealAdjacentSpaces(int col, int row)
	{
		if (col < 0 || col >= numCols || row < 0 || row >= numRows || !board[row][col].isHidden)
		{
			return;
		}

		// reveal the current space
		RevealSpace(col, row);

		// checking for adjacent spaces
		RevealAdjacentSpaces(col - 1, row); // left
		RevealAdjacentSpaces(col + 1, row); // right
		RevealAdjacentSpaces(col, row - 1); // up
		RevealAdjacentSpaces(col, row + 1); // down
	}

	bool IsGameWon() const
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; ++col)
			{
				if (!board[row][col].isMine && board[row][col].isHidden)
				{
					return false;
				}
			}
		}
		//if no hidden non-mine tiles are found, the game is won
		return true;
	}

	int NumFlagsPlaced() const
	{
		int count = 0;
		for (int i = 0; i < numRows; ++i)
		{
			for (int j = 0; j < numCols; ++j)
			{
				if (board[i][j].hasFlag)
				{
					++count;
				}
			}
		}
		return count;
	}


};


