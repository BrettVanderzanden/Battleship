#pragma once
#include "D2DRenderer.h"
#include "Ship.h"



enum Gamestate { eMenu, ePlay };
enum Playphase { eP1Placement, eP2Placement, eP1Attack, eP2Attack, eGameOver };
enum GameMode { eSinglePlayer, eTwoPlayer };
enum AIState { eRandomAttack, eKillShip };

class Game
{
	int GameBoard[2][10][10];
	Ship Ships[2][5];

	D2DRenderer *pRenderer = nullptr;

	// menu
	BVRect titleRect;
	BVRect singlePlayerButton;
	BVRect twoPlayerButton;

	// game
	BVRect topGrid;
	BVRect bottomGrid;
	BVRect infoRectTop;
	BVRect infoRectBottom;
	BVRect turnInfoRect;
	BVRect enemyShipsRemaining;
	BVRect enemyShipStatus[5];
	BVRect topTitle, bottomTitle;
	BVRect upperLetters[10];
	BVRect upperNumbers[10];
	BVRect lowerLetters[10];
	BVRect lowerNumbers[10];
	BVRect randomize;
	// ai
	AIState currentAIState;
	BVPoint lastAIHit;

	// gameover
	BVRect newGame;
	BVRect exitGame;

	int currentShipToPlace;
	int winner = 0;

	short spacePrev = 1;
	short spaceCurr = 1;

	std::wstring noticeText[2];

	void ClickPlacement(BVPoint mousePos, int player);
	void ClickGameplay(BVPoint mousePos, int player);
	void ClickGameOver(BVPoint mousePos);

	void DrawMenu();
	void DrawPlacementPhase();
	void DrawGameplay(int player);
	void DrawGameOver();
	void NextPhase();
	void HideShips(int player);
	void ShowShips(int player);
	void DamageShip(int player, int ship);
	bool ValidAttackPosition(int player, int x, int y);
	bool AlreadyAttacked(int target);
	void CheckWin();
	void Reset();
	void ClearBoard(int player);
	bool CheckValidPlacement(int player, int ship, int x, int y);
	void PlaceShip(int player, int ship, int x, int y);
	int AttackLocation(int playerAttacked, int x, int y);
	void RandomizeShips(int player);
	void AttackRandom(int player);
	void AIAttack();
	void GetAdjacentPositions(BVPoint loc, BVPoint *ptArr);

public:
	Game();
	~Game();

	bool Run();
	void Render(HWND hWnd);
	void Click(BVPoint mousePos);

	Gamestate currentGameState;
	Playphase currentPhase;
	GameMode currentMode;
};

