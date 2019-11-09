#ifndef People_h
#define People_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <String>
#include <vector>
#include "Anim.h"


using namespace sf;

class People
{
public:
	int healh;
	float x, y, dx, dy;
	FloatRect size;
	bool life;
	String file;

	People(FloatRect size)
	{
		life = true;
		this-> size = size;
		this->file = file;
		this->x = size.left;
		this->y = size.top;
		
	}
	

};

#endif