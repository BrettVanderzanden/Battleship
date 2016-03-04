#include "stdafx.h"
#include "Ship.h"

Ship::Ship()
{
}

Ship::~Ship()
{
}

Ship::Ship(ShipType _type, int _player)
{
	Init(_type, _player);
}

void Ship::Init(ShipType _type, int _player)
{
	type = _type;
	player = _player;
	switch (type)
	{
	case eCarrier:
		name = L"Aircraft Carrier";
		size = 5;
		break;
	case eBattleship:
		name = L"Battleship";
		size = 4;
		break;
	case eSubmarine:
		name = L"Submarine";
		size = 3;
		break;
	case eDestroyer:
		name = L"Destroyer";
		size = 3;
		break;
	case ePatrol:
		name = L"Patrol Boat";
		size = 2;
		break;
	default:
		break;
	}
	health = size;
	rect = BVRect(0.0f, 0.0f, SHIP_SIZE, SHIP_SIZE * size);
	horizontal = false;
	render = false;
}

void Ship::Rotate()
{
	float right, bottom;
	right = rect.right;
	bottom = rect.bottom;
	rect.bottom = rect.top + (right - rect.left);
	rect.right = rect.left + (bottom - rect.top);
	horizontal = !horizontal;
}

void Ship::Move(POINT screenPoint)
{
	rect.left = screenPoint.x - 15.0f;
	rect.top = screenPoint.y - 15.0f;
	if (horizontal)
	{
		rect.right = rect.left + SHIP_SIZE * size;
		rect.bottom = rect.top + SHIP_SIZE;
	}
	else
	{
		rect.right = rect.left + SHIP_SIZE;
		rect.bottom = rect.top + SHIP_SIZE * size;
	}
}

void Ship::PlaceOnBoard(int x, int y)
{
	position.x = (float)x;
	position.y = (float)y;
	if (horizontal)
	{
		rect.left = BOARD_LEFT + (float)x * SHIP_SIZE;
		rect.right = rect.left + (float)size * SHIP_SIZE;
		rect.top = LOWER_BOARD_TOP + y * SHIP_SIZE;
		rect.bottom = rect.top + SHIP_SIZE;
	}
	else
	{
		rect.left = BOARD_LEFT + x * SHIP_SIZE;
		rect.right = rect.left + SHIP_SIZE;
		rect.top = LOWER_BOARD_TOP + y * SHIP_SIZE;
		rect.bottom = rect.top + (float)size * SHIP_SIZE;
	}
}