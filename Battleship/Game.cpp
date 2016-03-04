#include "stdafx.h"
#include <sstream>
#include "Game.h"

Game::Game()
{
	pRenderer = D2DRenderer::GetInstance();
	
	// menu
	titleRect = BVRect(140, 50, 500, 200);
	singlePlayerButton = BVRect( 150, 200, 350, 300 );
	twoPlayerButton = BVRect( 150, 400, 350, 500 );

	// game
	topGrid = BVRect(BOARD_LEFT, UPPER_BOARD_TOP, BOARD_RIGHT, UPPER_BOARD_BOTTOM);
	bottomGrid = BVRect(BOARD_LEFT, LOWER_BOARD_TOP, BOARD_RIGHT, LOWER_BOARD_BOTTOM);
	infoRectTop = BVRect(0, 0, 400, 30);
	infoRectBottom = BVRect(0, 800, 400, 900);
	turnInfoRect = BVRect(100, 30, 300, 80);
	enemyShipsRemaining = BVRect(0, 500, 150, 600);
	for (size_t i = 0; i < 5; i++)
	{
		enemyShipStatus[i].left = BOARD_RIGHT + 5;
		enemyShipStatus[i].top = UPPER_BOARD_TOP + (float)i * 55;
		enemyShipStatus[i].right = enemyShipStatus[i].left + 50;
		enemyShipStatus[i].bottom = enemyShipStatus[i].top + 50;
	}
	topTitle = BVRect(BOARD_LEFT - SHIP_SIZE, UPPER_BOARD_TOP - 55, 200, UPPER_BOARD_TOP);
	bottomTitle = BVRect(BOARD_LEFT - SHIP_SIZE, LOWER_BOARD_TOP - 55, 200, LOWER_BOARD_TOP);
	for (size_t i = 0; i < 10; i++)
	{
		upperLetters[i].left = BOARD_LEFT + SHIP_SIZE * i;
		upperLetters[i].top = UPPER_BOARD_TOP - SHIP_SIZE;
		upperLetters[i].right = upperLetters[i].left + SHIP_SIZE;
		upperLetters[i].bottom = UPPER_BOARD_TOP;

		lowerLetters[i].left = BOARD_LEFT + SHIP_SIZE * i;
		lowerLetters[i].top = LOWER_BOARD_TOP - SHIP_SIZE;
		lowerLetters[i].right = lowerLetters[i].left + SHIP_SIZE;
		lowerLetters[i].bottom = LOWER_BOARD_TOP;

		upperNumbers[i].left = BOARD_LEFT - SHIP_SIZE;
		upperNumbers[i].top = UPPER_BOARD_TOP + i * SHIP_SIZE;
		upperNumbers[i].right = BOARD_LEFT;
		upperNumbers[i].bottom = upperNumbers[i].top + SHIP_SIZE;

		lowerNumbers[i].left = BOARD_LEFT - SHIP_SIZE;
		lowerNumbers[i].top = LOWER_BOARD_TOP + i * SHIP_SIZE;
		lowerNumbers[i].right = BOARD_LEFT;
		lowerNumbers[i].bottom = lowerNumbers[i].top + SHIP_SIZE;
	}
	randomize = BVRect(BOARD_RIGHT + 10, LOWER_BOARD_TOP + 100, BOARD_RIGHT + 120, LOWER_BOARD_TOP + 160);

	// gameover
	newGame = BVRect(50, 300, 150, 400);
	exitGame = BVRect(200, 300, 350, 400);

	Reset();
}

Game::~Game()
{
}

void Game::Reset()
{
	currentGameState = eMenu;
	currentPhase = eP1Placement;
	currentMode = eSinglePlayer;
	currentAIState = eRandomAttack;

	// game boards
	ClearBoard(0);
	ClearBoard(1);

	// ships
	for (size_t i = 0; i < 5; i++)
	{
		Ships[0][i].Init((ShipType)i, 0);
		Ships[1][i].Init((ShipType)i, 1);
	}
}

void Game::ClearBoard(int player)
{
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			GameBoard[player][i][j] = EMPTY_SEA;
		}
	}
}

bool Game::Run()
{
	spacePrev = spaceCurr;
	spaceCurr = GetAsyncKeyState(VK_SPACE);

	if (currentGameState == ePlay)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(pRenderer->MainhWnd, &p);
		if (currentPhase == eP1Placement)
		{
			Ships[0][currentShipToPlace].Move(p);
			if (spaceCurr && !spacePrev)
			{
				Ships[0][currentShipToPlace].Rotate();
			}
		}
		else if (currentPhase == eP2Placement)
		{
			Ships[1][currentShipToPlace].Move(p);
			if (spaceCurr && !spacePrev)
			{
				Ships[1][currentShipToPlace].Rotate();
			}
		}
	}
	return true;
}

void Game::Click(BVPoint mousePos)
{
	if (currentGameState == eMenu)
	{
		if (CheckCollision(singlePlayerButton, mousePos))
		{
			currentGameState = ePlay;
			currentMode = eSinglePlayer;
			Ships[0][0].render = true;
		}
		if (CheckCollision(twoPlayerButton, mousePos))
		{
			currentGameState = ePlay;
			currentMode = eTwoPlayer;
			Ships[0][0].render = true;
		}
	}
	else
	{
		switch (currentPhase)
		{
		case eP1Placement:
			if (CheckCollision(randomize, mousePos))
			{
				RandomizeShips(0);
				NextPhase();
			}
			else
			{
				ClickPlacement(mousePos, 0);
			}
			break;
		case eP2Placement:
			if (CheckCollision(randomize, mousePos))
			{
				RandomizeShips(1);
				NextPhase();
			}
			else
			{
				ClickPlacement(mousePos, 1);
			}
			break;
		case eP1Attack:
			if (CheckCollision(randomize, mousePos))
			{
				RandomizeShips(1);
				NextPhase();
			}
			else
			{
				ClickGameplay(mousePos, 0);
			}
			break;
		case eP2Attack:
			if (CheckCollision(randomize, mousePos))
			{
				RandomizeShips(1);
				NextPhase();
			}
			else
			{
				ClickGameplay(mousePos, 1);
			}
			break;
		case eGameOver:
			ClickGameOver(mousePos);
			break;
		default:
			break;
		}
	}
}

void Game::ClickPlacement(BVPoint mousePos, int player)
{
	int xPos, yPos;
	xPos = ((int)mousePos.x - BOARD_LEFT) / (int)SHIP_SIZE;
	yPos = ((int)mousePos.y - LOWER_BOARD_TOP) / (int)SHIP_SIZE;

	if (CheckCollision(bottomGrid, BVPoint(mousePos.x, mousePos.y)) && CheckValidPlacement(player, currentShipToPlace, xPos, yPos))
	{
		PlaceShip(player, currentShipToPlace, xPos, yPos);
		currentShipToPlace++;
		if (currentShipToPlace > 4)
		{
			NextPhase();
			currentShipToPlace = 0;
		}
		else
		{
			Ships[player][currentShipToPlace].render = true;
		}
	}
}

void Game::ClickGameplay(BVPoint mousePos, int player)
{
	int xPos, yPos;
	xPos = ((int)mousePos.x - BOARD_LEFT) / (int)SHIP_SIZE;
	yPos = ((int)mousePos.y - UPPER_BOARD_TOP) / (int)SHIP_SIZE;
	int target;
	if (CheckCollision(topGrid, BVPoint(mousePos.x, mousePos.y)))
	{
		target = AttackLocation(player, xPos, yPos);
	}
	if (!AlreadyAttacked(target))
	{
		CheckWin();
		NextPhase();
	}
}

void Game::ClickGameOver(BVPoint mousePos)
{
	if (CheckCollision(newGame, mousePos))
	{
		Reset();
	}
	if (CheckCollision(exitGame, mousePos))
	{
		// exit game
	}
}

void Game::NextPhase()
{
	switch (currentPhase)
	{
	case eP1Placement:
		if (currentMode == eSinglePlayer)
		{
			RandomizeShips(1);
			currentPhase = eP1Attack;
			currentGameState = ePlay;
		}
		else // 2 player mode
		{
			currentPhase = eP2Placement;
			HideShips(0);
			Ships[1][0].render = true;
		}
		break;
	case eP2Placement:
		currentPhase = eP1Attack;
		currentGameState = ePlay;
		break;
	case eP1Attack:
		if (currentMode == eTwoPlayer)
		{
			currentPhase = eP2Attack;
			HideShips(0);
			ShowShips(1);
		}
		else
		{
			AIAttack();
		}
		break;
	case eP2Attack:
		currentPhase = eP1Attack;
		HideShips(1);
		ShowShips(0);
		break;
	default:
		break;
	}
}

int Game::AttackLocation(int player, int x, int y)
{
	int enemyPlayer = player ^ 1;
	int target = GameBoard[enemyPlayer][y][x];
	if (!AlreadyAttacked(target))
	{
		if (target != EMPTY_SEA)
		{
			GameBoard[enemyPlayer][y][x] = ATTACK_HIT;
			std::wostringstream notice;
			notice << L"Player " << player + 1 << " hit " << Ships[enemyPlayer][target].name;
			noticeText[enemyPlayer] = notice.str();
			DamageShip(enemyPlayer, target);
		}
		else
		{
			GameBoard[enemyPlayer][y][x] = ATTACK_MISS;
			std::wostringstream notice;
			notice << L"Player " << player + 1 << " missed";
			noticeText[enemyPlayer] = notice.str();
		}
	}
	return target;
}

void Game::AttackRandom(int player)
{

}

bool Game::ValidAttackPosition(int player, int x, int y)
{
	if (x > -1 && x < 10 && y > -1 && y < 10)
	{
		if (GameBoard[player][y][x] == ATTACK_HIT || GameBoard[player][y][x] == ATTACK_MISS)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool Game::AlreadyAttacked(int target)
{
	if (target == ATTACK_HIT || target == ATTACK_MISS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Game::DamageShip(int player, int ship)
{
	int enemyPlayer = player ^ 1;
	Ships[player][ship].health--;
	if (Ships[player][ship].health == 0)
	{
		// alert other player ship is killed
		std::wostringstream notice;
		notice << L"Player " << enemyPlayer + 1 << " destroyed " << Ships[player][ship].name;
		noticeText[player] = notice.str();
	}
}

void Game::CheckWin()
{
	bool p1win, p2win;
	p1win = p2win = true;
	for (size_t i = 0; i < 5; i++)
	{
		if (Ships[0][i].health > 0)
		{
			p2win = false;
		}
	}
	for (size_t i = 0; i < 5; i++)
	{
		if (Ships[1][i].health > 0)
		{
			p1win = false;
		}
	}
	if (p1win)
	{
		currentPhase = eGameOver;
		winner = 0;
	}
	else if (p2win)
	{
		currentPhase = eGameOver;
		winner = 1;
	}
}

bool Game::CheckValidPlacement(int player, int ship, int x, int y)
{
	bool validPlacement = true;
	if (Ships[player][ship].horizontal)
	{
		for (int i = 0; i < Ships[player][ship].size; i++)
		{
			if (x + i > 9 || GameBoard[player][y][x + i] != EMPTY_SEA)
			{
				validPlacement = false;
			}
		}
	}
	else // !horizontal
	{
		for (int i = 0; i < Ships[player][ship].size; i++)
		{
			if (y + i > 9 || GameBoard[player][y + i][x] != EMPTY_SEA)
			{
				validPlacement = false;
			}
		}
	}
	return validPlacement;
}

void Game::PlaceShip(int player, int ship, int x, int y)
{
	if (Ships[player][ship].horizontal)
	{
		for (int i = 0; i < Ships[player][ship].size; i++)
		{
			GameBoard[player][y][x + i] = (int)Ships[player][ship].type;
		}
	}
	else
	{
		for (int i = 0; i < Ships[player][ship].size; i++)
		{
			GameBoard[player][y + i][x] = (int)Ships[player][ship].type;
		}
	}
	Ships[player][ship].PlaceOnBoard(x, y);
}

void Game::RandomizeShips(int player)
{
	ClearBoard(player);
	for (size_t i = 0; i < 5; i++)
	{
		int horizontal = rand() % 2;
		Ships[player][i].horizontal = horizontal != 0;
		bool valid = false;
		while (!valid)
		{
			int x, y;
			x = rand() % 10;
			y = rand() % 10;
			if (CheckValidPlacement(player, i, x, y))
			{
				PlaceShip(player, i, x, y);
				valid = true;
			}
		}
	}
}

void Game::AIAttack()
{
	int target = -1;
	BVPoint atkLoc;
	//if (currentAIState == eRandomAttack)
	{
		bool valid = false;
		while (!valid)
		{
			atkLoc = BVPoint(rand() % 10, rand() % 10);
			if (ValidAttackPosition(0, (int)atkLoc.x, (int)atkLoc.y))
			{
				target = AttackLocation(1, (int)atkLoc.x, (int)atkLoc.y);
				valid = true;
			}
		}
	}
	//else // eKillShip
	//{
	//	BVPoint adjacent[4];
	//	GetAdjacentPositions(lastAIHit, adjacent);
	//	bool attacked = false;
	//	for (size_t i = 0; i < 4; i++)
	//	{
	//		if (ValidAttackPosition(0, (int)adjacent[i].x, (int)adjacent[i].y))
	//		{
	//			target = AttackLocation(1, (int)adjacent[i].x, (int)adjacent[i].y);
	//			attacked = true;
	//		}
	//	}
	//	if (!attacked)
	//	{
	//		currentAIState = eRandomAttack;
	//		AIAttack();
	//		return;
	//	}
	//}
	
	if (target >= 0)
	{
		lastAIHit = atkLoc;
		if (Ships[0][target].health == 0)
		{
			currentAIState = eRandomAttack;
		}
		else
		{
			currentAIState = eKillShip;
		}
		CheckWin();
	}
}

void Game::GetAdjacentPositions(BVPoint loc, BVPoint *ptArr)
{
	ptArr[0] = ptArr[1] = ptArr[2] = ptArr[3] = loc;
	ptArr[0].y -= 1;
	ptArr[1].y += 1;
	ptArr[2].x -= 1;
	ptArr[3].x += 1;
}

void Game::Render(HWND hWnd)
{
	pRenderer->pRT->BeginDraw();

	// Set all pixels on the Render Target's Back Buffer to the specified color
	pRenderer->pRT->Clear(pRenderer->bkgndColor);

	///////////////////////////////////////////////////
	//          TODO: Begin drawing code             //
	///////////////////////////////////////////////////
	if (currentGameState == eMenu)
	{
		DrawMenu();
	}
	else
	{
		if (currentPhase == eP1Placement || currentPhase == eP2Placement)
		{
			DrawPlacementPhase();
		}
		else if (currentPhase == eP1Attack)
		{
			DrawGameplay(0);
		}
		else if (currentPhase == eP2Attack)
		{
			DrawGameplay(1);
		}
		else if (currentPhase == eGameOver)
		{
			if (currentMode == eTwoPlayer)
			{
				DrawGameplay(winner);
			}
			else
			{
				DrawGameplay(0);
			}
			DrawGameOver();
		}
	}
	///////////////////////////////////////////////////
	//              End drawing code                 //
	///////////////////////////////////////////////////

	HRESULT hr = pRenderer->pRT->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		//Destroy Direct2D.
		pRenderer->DestroyGraphics();
		//Recreate Direct2D.
		pRenderer->CreateGraphics(hWnd);
	}

	ValidateRect(hWnd, NULL); // VERY IMPORTANT: Must clear the invalid flag.
}

void Game::HideShips(int player)
{
	for (size_t i = 0; i < 5; i++)
	{
		Ships[player][i].render = false;
	}
}

void Game::ShowShips(int player)
{
	for (size_t i = 0; i < 5; i++)
	{
		Ships[player][i].render = true;
	}
}

void Game::DrawMenu()
{ 
	pRenderer->pRT->DrawRectangle(singlePlayerButton.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->DrawRectangle(twoPlayerButton.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->DrawTextW(L"BATTLESHIP", 10, pRenderer->pTitleText, titleRect.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"Single Player", 13, pRenderer->pInfoText, singlePlayerButton.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"Two Player", 10, pRenderer->pInfoText, twoPlayerButton.D2D(), pRenderer->blackBrush);
}

void Game::DrawPlacementPhase()
{
	std::wostringstream shipToPlace;
	shipToPlace << L"Place " << Ships[0][currentShipToPlace].name << L" on the board.";
	std::wstring howTo = L"Press SPACE to rotate.";
	pRenderer->pRT->DrawTextW(howTo.c_str(), howTo.length(), pRenderer->pInfoText, infoRectTop.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(shipToPlace.str().c_str(), shipToPlace.str().length(), pRenderer->pInfoText, infoRectBottom.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"Your board", 10, pRenderer->pInfoText, bottomTitle.D2D(), pRenderer->blackBrush);

	// grid
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT + (float)i * 30.0f;
		start.y = LOWER_BOARD_TOP - SHIP_SIZE;
		finish.x = start.x;
		finish.y = LOWER_BOARD_BOTTOM;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT - SHIP_SIZE;
		start.y = LOWER_BOARD_TOP + (float)i * 30;
		finish.x = BOARD_RIGHT;
		finish.y = start.y;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	// letters/numbers
	for (size_t i = 0; i < 10; i++)
	{
		std::wstring num, letter;
		num = std::to_wstring(i + 1);
		letter = (wchar_t)i + 65;
		pRenderer->pRT->DrawTextW(num.c_str(), num.length(), pRenderer->pInfoText, lowerNumbers[i].D2D(), pRenderer->blackBrush);
		pRenderer->pRT->DrawTextW(letter.c_str(), letter.length(), pRenderer->pInfoText, lowerLetters[i].D2D(), pRenderer->blackBrush);
	}

	for (size_t i = 0; i < 5; i++)
	{
		if (Ships[0][i].render)
		{
			pRenderer->pRT->FillRectangle(Ships[0][i].rect.D2D(), pRenderer->grayBrush);
			pRenderer->pRT->DrawRectangle(Ships[0][i].rect.D2D(), pRenderer->darkGrayBrush);
		}
		if (Ships[1][i].render)
		{
			pRenderer->pRT->FillRectangle(Ships[1][i].rect.D2D(), pRenderer->grayBrush);
			pRenderer->pRT->DrawRectangle(Ships[1][i].rect.D2D(), pRenderer->darkGrayBrush);
		}
	}

	// randomize
	pRenderer->pRT->FillRectangle(randomize.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->DrawTextW(L"Randomize", 9, pRenderer->pInfoText, randomize.D2D(), pRenderer->blackBrush);
}

void Game::DrawGameplay(int player)
{
	std::wstring howTo = L"Select a position to attack";
	pRenderer->pRT->DrawTextW(howTo.c_str(), howTo.length(), pRenderer->pInfoText, infoRectTop.D2D(), pRenderer->blackBrush);

	pRenderer->pRT->DrawTextW(noticeText[player].c_str(), noticeText[player].length(), pRenderer->pInfoText, infoRectBottom.D2D(), pRenderer->blackBrush);

	pRenderer->pRT->DrawTextW(L"Your board", 10, pRenderer->pInfoText, bottomTitle.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"Enemy board", 11, pRenderer->pInfoText, topTitle.D2D(), pRenderer->blackBrush);


	std::wostringstream turnInfo;
	turnInfo << L"Player " << player + 1  << "'s turn";
	pRenderer->pRT->DrawTextW(turnInfo.str().c_str(), turnInfo.str().length(), pRenderer->pInfoText, turnInfoRect.D2D(), pRenderer->blackBrush);

	// upper grid
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT + (float)i * 30.0f;
		start.y = UPPER_BOARD_TOP - SHIP_SIZE;
		finish.x = start.x;
		finish.y = UPPER_BOARD_BOTTOM;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT - SHIP_SIZE;
		start.y = UPPER_BOARD_TOP + (float)i * 30;
		finish.x = BOARD_RIGHT;
		finish.y = start.y;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	// lower grid
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT + (float)i * 30.0f;
		start.y = LOWER_BOARD_TOP - SHIP_SIZE;
		finish.x = start.x;
		finish.y = LOWER_BOARD_BOTTOM;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	for (size_t i = 0; i < 11; i++)
	{
		D2D1_POINT_2F start, finish;
		start.x = BOARD_LEFT - SHIP_SIZE;
		start.y = LOWER_BOARD_TOP + (float)i * 30;
		finish.x = BOARD_RIGHT;
		finish.y = start.y;
		pRenderer->pRT->DrawLine(start, finish, pRenderer->blackBrush, 0.5f);
	}
	// letters/numbers
	for (size_t i = 0; i < 10; i++)
	{
		std::wstring num, letter;
		num = std::to_wstring(i + 1);
		letter = (wchar_t)i + 65;
		pRenderer->pRT->DrawTextW(num.c_str(), num.length(), pRenderer->pInfoText, upperNumbers[i].D2D(), pRenderer->blackBrush);
		pRenderer->pRT->DrawTextW(num.c_str(), num.length(), pRenderer->pInfoText, lowerNumbers[i].D2D(), pRenderer->blackBrush);
		pRenderer->pRT->DrawTextW(letter.c_str(), letter.length(), pRenderer->pInfoText, upperLetters[i].D2D(), pRenderer->blackBrush);
		pRenderer->pRT->DrawTextW(letter.c_str(), letter.length(), pRenderer->pInfoText, lowerLetters[i].D2D(), pRenderer->blackBrush);
	}

	// draw ships and enemy ship info
	int enemyPlayer = player ^ 1;
	for (size_t i = 0; i < 5; i++)
	{
		pRenderer->pRT->FillRectangle(Ships[player][i].rect.D2D(), pRenderer->grayBrush);
		pRenderer->pRT->DrawRectangle(Ships[player][i].rect.D2D(), pRenderer->darkGrayBrush);

		pRenderer->pRT->FillRectangle(enemyShipStatus[i].D2D(), pRenderer->grayBrush);

		pRenderer->pRT->DrawTextW(Ships[enemyPlayer][i].name.c_str(), Ships[enemyPlayer][i].name.size(), pRenderer->pSmallText, enemyShipStatus[i].D2D(), pRenderer->blackBrush);
		if (Ships[enemyPlayer][i].health == 0)
		{
			pRenderer->pRT->DrawTextW(L"X", 1, pRenderer->pTitleText, enemyShipStatus[i].D2D(), pRenderer->redBrush);
		}

	}

	// draw hits/misses
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			D2D1_ELLIPSE ellipse;
			ellipse.radiusX = 5;
			ellipse.radiusY = 5;
			if (GameBoard[player][y][x] == ATTACK_HIT)
			{
				ellipse.point.x = BOARD_LEFT + ((float)x * SHIP_SIZE) + (SHIP_SIZE / 2);
				ellipse.point.y = LOWER_BOARD_TOP + ((float)y * SHIP_SIZE) + (SHIP_SIZE / 2);
				pRenderer->pRT->FillEllipse(ellipse, pRenderer->redBrush);
			}
			else if (GameBoard[player][y][x] == ATTACK_MISS)
			{
				ellipse.point.x = BOARD_LEFT + ((float)x * SHIP_SIZE) + (SHIP_SIZE / 2);
				ellipse.point.y = LOWER_BOARD_TOP + ((float)y * SHIP_SIZE) + (SHIP_SIZE / 2);
				pRenderer->pRT->FillEllipse(ellipse, pRenderer->whiteBrush);
			}
			if (GameBoard[enemyPlayer][y][x] == ATTACK_HIT)
			{
				ellipse.point.x = BOARD_LEFT + ((float)x * SHIP_SIZE) + (SHIP_SIZE / 2);
				ellipse.point.y = UPPER_BOARD_TOP + ((float)y * SHIP_SIZE) + (SHIP_SIZE / 2);
				pRenderer->pRT->FillEllipse(ellipse, pRenderer->redBrush);
			}
			else if (GameBoard[enemyPlayer][y][x] == ATTACK_MISS)
			{
				ellipse.point.x = BOARD_LEFT + ((float)x * SHIP_SIZE) + (SHIP_SIZE / 2);
				ellipse.point.y = UPPER_BOARD_TOP + ((float)y * SHIP_SIZE) + (SHIP_SIZE / 2);
				pRenderer->pRT->FillEllipse(ellipse, pRenderer->whiteBrush);
			}
		}
	}

	// randomize
	pRenderer->pRT->FillRectangle(randomize.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->DrawTextW(L"Attack\nRandom", 13, pRenderer->pInfoText, randomize.D2D(), pRenderer->blackBrush);
}

void Game::DrawGameOver()
{
	pRenderer->pRT->FillRectangle(newGame.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->FillRectangle(exitGame.D2D(), pRenderer->blueBrush);
	pRenderer->pRT->DrawTextW(L"Game Over", 10, pRenderer->pTitleText, titleRect.D2D(), pRenderer->blackBrush);
	std::wostringstream winnerstring;
	winnerstring << L"Player  " << (winner + 1) << L" wins!";
	D2D1_RECT_F winnerRect = titleRect.D2D();
	winnerRect.top += 60;
	winnerRect.bottom += 30;
	pRenderer->pRT->DrawTextW(winnerstring.str().c_str(), winnerstring.str().size() , pRenderer->pInfoText, winnerRect, pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"New Game", 8, pRenderer->pInfoText, newGame.D2D(), pRenderer->blackBrush);
	pRenderer->pRT->DrawTextW(L"Exit", 4, pRenderer->pInfoText, exitGame.D2D(), pRenderer->blackBrush);
}