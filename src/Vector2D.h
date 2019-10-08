#pragma once
#include <iostream>
#include <SDL.h>

#include "Logging.h"

class Vector2D
{
private:
	
public:
	Vector2D();

	Vector2D(int eX, int eY);
	
	~Vector2D();

	int x =0 ;
	int y = 0;
};
