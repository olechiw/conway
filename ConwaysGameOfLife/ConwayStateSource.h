#pragma once

#include "ConwayGame.h"

class ConwayStateSource
{
public:
	virtual ConwayGame::State getState() = 0;
};