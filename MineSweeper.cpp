#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include "Random.h"
#include "TextureManager.h"
#include "Board.h"
#include "enum_.h"
using namespace std;

void InitializeBoardFromFile(Board& board, const string& filename)
{
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // read the file and initialize the board
    for (int i = 0; i < board.numRows; ++i)
    {
        if (getline(file, line))
        {
            for (int j = 0; j < board.numCols && j < line.size(); ++j)
            {
                // '0' represents an empty space, and '1' represents a mine
                if (line[j] == '0')
                {
                    board.board[i][j].isMine = false;
                }
                else if (line[j] == '1')
                {
                    board.board[i][j].isMine = true;
                }
            }
        }
    }
    //board.minesRemaining = board.numMines;
    file.close();
}

Board ReadBoardFile(string configFile)
{
    ifstream file(configFile);
    string line;
    if (!file.is_open())
    {
        cerr << "Error opening file: " << configFile << endl;
        Board board;
        return board;
    }

    getline(file, line);
    int numCols = stoi(line);

    getline(file, line);
    int numRows = stoi(line);

    getline(file, line);
    int numMines = stoi(line);

    Board board(numCols, numRows, numMines);
    board.minesRemaining = numMines;


    return board;
}


bool isMouseClicked(sf::Sprite& sprite, sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

// for testing buttons
void InitializeTest(Board& board, const string& configFile)
{
    //board.RestartBoard();
    InitializeBoardFromFile(board, configFile);
}


GameState gameState = GameState::PLAYING;


int main()
{
    unordered_map<string, sf::Texture> textures;
    sf::Sprite tileHidden(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite tileRevealed(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));
    sf::Sprite faceWinSprite(TextureManager::GetTexture("face_win"));
    sf::Sprite faceLoseSprite(TextureManager::GetTexture("face_lose"));
    sf::Sprite mineSprite(TextureManager::GetTexture("mine"));
    sf::Sprite num1Sprite(TextureManager::GetTexture("number_1"));
    sf::Sprite num2Sprite(TextureManager::GetTexture("number_2"));
    sf::Sprite num3Sprite(TextureManager::GetTexture("number_3"));
    sf::Sprite num4Sprite(TextureManager::GetTexture("number_4"));
    sf::Sprite num5Sprite(TextureManager::GetTexture("number_5"));
    sf::Sprite num6Sprite(TextureManager::GetTexture("number_6"));
    sf::Sprite num7Sprite(TextureManager::GetTexture("number_7"));
    sf::Sprite num8Sprite(TextureManager::GetTexture("number_8"));
    sf::Sprite digitsSprite(TextureManager::GetTexture("digits"));


    auto board = ReadBoardFile("boards/config.cfg");
    sf::RenderWindow window(sf::VideoMode(board.numCols * 32, board.numRows * 32 + 100), "SFML works!");

    sf::Sprite faceHappySprite(TextureManager::GetTexture("face_happy"));
    faceHappySprite.setPosition(800 / 2 + 16, 600 - 84);

    sf::Sprite debugSprite(TextureManager::GetTexture("debug"));
    debugSprite.setPosition(800 / 2 + 144, 600 - 84);

    sf::Sprite test1Sprite(TextureManager::GetTexture("test_1"));
    test1Sprite.setPosition(800 / 2 + 208, 600 - 84);

    sf::Sprite test2Sprite(TextureManager::GetTexture("test_2"));
    test2Sprite.setPosition(800 / 2 + 208 + 64, 600 - 84);

    sf::Sprite test3Sprite(TextureManager::GetTexture("test_3"));
    test3Sprite.setPosition(800 / 2 + 208 + 64 + 64, 600 - 84);


    board.InitializeBoardWithMines(board);
    board.minesRemaining = board.numMines;
    bool mineClicked = false;
    bool gameEnded = false;
    bool debugButtonClicked = false;
    bool testButtonClicked = false;
    GameState gameState = GameState::PLAYING;


    while (window.isOpen())
    {
        if (board.IsGameWon())
        {
            gameState = GameState::WIN;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            // HANDLING GAME LOGIC
            if (!gameEnded)
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                        //get the row and column the user clicked on
                        int row = static_cast<int>(sf::Mouse::getPosition(window).y / 32.0f);
                        int col = static_cast<int>(sf::Mouse::getPosition(window).x / 32.0f);

                        if (isMouseClicked(faceHappySprite, window))
                        {
                            gameState = GameState::PLAYING;
                            mineClicked = false;
                            gameEnded = false;
                            testButtonClicked = false;
                            board.RestartBoard();
                            board.debugMode = false;
                        }

                        // TESTING BUTTONS
                        else if (isMouseClicked(test1Sprite, window))
                        {
                            gameState = GameState::PLAYING;
                            mineClicked = false;
                            gameEnded = false;
                            board.RestartBoard();
                            board.debugMode = false;
                            InitializeTest(board, "boards/testboard1.brd");
                        }
                        else if (isMouseClicked(test2Sprite, window))
                        {
                            gameState = GameState::PLAYING;
                            mineClicked = false;
                            gameEnded = false;
                            board.RestartBoard();
                            board.debugMode = false;
                            InitializeTest(board, "boards/testboard2.brd");
                        }
                        else if (isMouseClicked(test3Sprite, window))
                        {
                            gameState = GameState::PLAYING;
                            mineClicked = false;
                            gameEnded = false;
                            board.RestartBoard();
                            board.debugMode = false;
                            InitializeTest(board, "boards/testboard3.brd");
                        }

                        if (row < board.numRows && col < board.numCols && board.board[row][col].isHidden)
                        {
                            // checks if the tile has a mine
                            if (board.board[row][col].isMine)
                            {
                                // reveal the clicked mine
                                board.board[row][col].isHidden = false;

                                // reveal all mines on the board
                                for (int r = 0; r < board.numRows; ++r)
                                {
                                    for (int c = 0; c < board.numCols; ++c)
                                    {
                                        if (board.board[r][c].isMine)
                                        {
                                            board.board[r][c].isHidden = false;
                                            board.board[r][c].hasFlag = false;
                                        }
                                    }
                                }

                                mineClicked = true;
                                gameEnded = true;
                            }

                            else
                            {
                                board.AdjacentMines();

                                if (board.board[row][col].adjacentMines == 0)
                                {
                                    // recursively reveal adjacent spaces
                                    board.RevealAdjacentSpaces(col, row);
                                }
                                else
                                {
                                    // reveal the clicked space
                                    board.RevealSpace(col, row);
                                }


                            }

                        }

                    }

                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        //get the row and column the user clicked on
                        int row = (int)sf::Mouse::getPosition(window).y / 32.0f;
                        int col = (int)sf::Mouse::getPosition(window).x / 32.0f;
                        board.SetFlag(col, row);
                    }
                }
            }
        }


        window.clear(sf::Color::White);

        // REPLACE THE FACES TO LOSING AND WINNING/USE TEST BUTTONS POST WIN OR LOSE

        if (mineClicked)
        {
            faceLoseSprite.setTexture(TextureManager::GetTexture("face_lose"));
            faceLoseSprite.setPosition(faceHappySprite.getPosition());
            window.draw(faceLoseSprite);

            if (isMouseClicked(faceLoseSprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
            }

            else if (isMouseClicked(test1Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard1.brd");
            }
            else if (isMouseClicked(test2Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard2.brd");
            }
            else if (isMouseClicked(test3Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard3.brd");
            }
        }
        else if (gameState == GameState::WIN)
        {
            faceWinSprite.setTexture(TextureManager::GetTexture("face_win"));
            faceWinSprite.setPosition(faceHappySprite.getPosition());
            window.draw(faceWinSprite);

            for (int r = 0; r < board.numRows; ++r)
            {
                for (int c = 0; c < board.numCols; ++c)
                {
                    if (board.board[r][c].isMine)
                    {
                        board.board[r][c].hasFlag = true;
                    }
                }
            }

            if (isMouseClicked(faceWinSprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
            }

            else if (isMouseClicked(test1Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard1.brd");
            }
            else if (isMouseClicked(test2Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard2.brd");
            }
            else if (isMouseClicked(test3Sprite, window))
            {
                gameState = GameState::PLAYING;
                mineClicked = false;
                gameEnded = false;
                board.RestartBoard();
                board.debugMode = false;
                InitializeTest(board, "boards/testboard3.brd");
            }
        }

        else
        {
            window.draw(faceHappySprite);
        }

        // DRAWING TILES

        for (auto& row : board.board)
        {
            for (auto& tile : row)
            {
                if (tile.isHidden)
                {
                    tileHidden.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                    window.draw(tileHidden);
                }
                else
                {
                    tileRevealed.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                    window.draw(tileRevealed);

                    if (!tile.isMine && tile.adjacentMines > 0 && !tile.isHidden)
                    {
                        int digit = tile.adjacentMines;
                        sf::Sprite digitSprite;

                        switch (digit)
                        {
                        case 1:
                            digitSprite = num1Sprite;
                            break;
                        case 2:
                            digitSprite = num2Sprite;
                            break;
                        case 3:
                            digitSprite = num3Sprite;
                            break;
                        case 4:
                            digitSprite = num4Sprite;
                            break;
                        case 5:
                            digitSprite = num5Sprite;
                            break;
                        case 6:
                            digitSprite = num6Sprite;
                            break;
                        case 7:
                            digitSprite = num7Sprite;
                            break;
                        case 8:
                            digitSprite = num8Sprite;
                            break;
                        default:
                            break;
                        }

                        digitSprite.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(digitSprite);
                    }
                }


                if (tile.hasFlag)
                {
                    flag.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                    window.draw(flag);
                }

                if (tile.isMine && !tile.isHidden)
                {
                    mineSprite.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                    window.draw(mineSprite);
                }
            }
        }

        // DEBUG MODE        
        if (isMouseClicked(debugSprite, window) && !debugButtonClicked && !mineClicked && !board.IsGameWon())
        {
            //cout << "Debug button clicked!" << endl;
            board.debugMode = !board.debugMode;
            cout << "Debug mode: " << (board.debugMode ? "ON" : "OFF") << endl;
            debugButtonClicked = true;
        }

        if (!isMouseClicked(debugSprite, window))
        {
            debugButtonClicked = false;
        }

        // draw mines over anything else if debug mode is active
        for (auto& row : board.board)
        {
            for (auto& tile : row)
            {
                if (!board.debugMode && tile.isHidden)
                {
                    // only draw hidden elements when debug mode is off
                    continue;
                }
            }
        }

        if (board.debugMode)
        {
            // draw mines or flags based on game state and debug mode
            for (auto& row : board.board)
            {
                for (auto& tile : row)
                {
                    if (gameState == GameState::WIN && tile.isMine && tile.isHidden)
                    {
                        // draw flags instead of mines when the game is won
                        flag.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(flag);
                    }
                    else if (!board.debugMode && tile.isHidden)
                    {
                        // draw hidden elements only when debug mode is off
                        continue;
                    }
                    else if (board.debugMode && tile.isMine && tile.isHidden)
                    {
                        // draw mines when debug mode is on
                        mineSprite.setPosition(tile.col * 32.0f, tile.row * 32.0f);
                        window.draw(mineSprite);
                    }
                }
            }
        }

        // PLACING FLAG COUNTER DIGITS
        int numFlagsPlaced = board.NumFlagsPlaced();
        int digitWidth = 21;
        int digitOffset = 21;

        int hundredsDigit = board.minesRemaining / 100;
        int tensDigit = (board.minesRemaining / 10) % 10;
        int onesDigit = board.minesRemaining % 10;

        if (gameState == GameState::WIN)
        {
            // display "0" for the mine counter when the game is won
            digitsSprite.setTextureRect(sf::IntRect(0, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370, 600 - 84);
            window.draw(digitsSprite);

            digitsSprite.setTextureRect(sf::IntRect(0, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370 + digitOffset, 600 - 84);
            window.draw(digitsSprite);

            digitsSprite.setTextureRect(sf::IntRect(0, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370 + 2 * digitOffset, 600 - 84);
            window.draw(digitsSprite);
        }

        else
        {
            digitsSprite.setTextureRect(sf::IntRect(hundredsDigit * digitWidth, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370, 600 - 84);
            window.draw(digitsSprite);

            digitsSprite.setTextureRect(sf::IntRect(tensDigit * digitWidth, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370 + digitOffset, 600 - 84);
            window.draw(digitsSprite);

            digitsSprite.setTextureRect(sf::IntRect(onesDigit * digitWidth, 0, digitWidth, 32));
            digitsSprite.setPosition(800 / 2 - 370 + 2 * digitOffset, 600 - 84);
            window.draw(digitsSprite);
        }

        //DRAWS BUTTONS
        window.draw(debugSprite);
        window.draw(test1Sprite);
        window.draw(test2Sprite);
        window.draw(test3Sprite);

        window.display();
    }

    TextureManager::Clear(); // cleaning up the mess we made
    return 0;
}




