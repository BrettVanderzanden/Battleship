#pragma once
#include "Math.h"

enum ShipType { eCarrier, eBattleship, eSubmarine, eDestroyer, ePatrol };

class Ship
{
public:
	Ship();
	~Ship();
	Ship(ShipType _type, int _player);

	void Init(ShipType _type, int _player);
	void Rotate();
	void Move(POINT screenPoint);
	void PlaceOnBoard(int x, int y);
	bool horizontal = false;
	bool render = false;

	int size = 0;
	int health = 0;
	int player;
	std::wstring name;
	BVRect rect;
	ShipType type;
	BVPoint position;
};

