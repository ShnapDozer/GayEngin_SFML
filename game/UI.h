#ifndef UI_h
#define UI_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <String>
#include <vector>



using namespace sf;

class Inventory 
{
private:
	Texture Predmet;
	Sprite Item;
	Texture Tex;
	Sprite Invent;
	String papka;
	FloatRect Size;
	bool isopen;
	std::map <int, int> inventer = {};
public:
	Inventory(String papka, bool isopen)
	{
		this->papka = papka;
		this->isopen = isopen;
		Tex.loadFromFile(papka+"inventori.png");
		Invent.setTexture(Tex);
		Size = Invent.getGlobalBounds();
		for (int i = 0; i < 12; i++) 
		{
			inventer[i] = 0;
		}
	}
	void update(Vector2f poscurs, int tx, int ty, int x, int y)
	{
		Invent.setPosition(tx - x / 2, ty - y / 2);
		Item.setPosition(tx - x / 2, ty - y / 2);
	}
	void add(int prd) 
	{
		inventer[prd] = 1;
	}
	void draw(RenderWindow &window)
	{
		window.draw(Invent);
		for (int i = 0; i < 12; i++)
		{
			if (inventer[i] == 1)
			{
				Predmet.loadFromFile(papka + numList[1]);
				Item.setTexture(Predmet);
				Item.move(0,100*i);
				std::cout << 100 * i << std::endl;
				window.draw(Item);
			}
		}
	}

};





#endif
