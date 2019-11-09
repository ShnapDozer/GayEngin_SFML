#ifndef Anim_h
#define Anim_h

//only png image!
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <String>
#include <vector>
using namespace sf;

std::string mirror = "flip";
std::map<int, std::string > numList =
{
	{0, "0.png"},
	{1, "1.png"},
	{2, "2.png"},
	{3, "3.png"},
	{4, "4.png"},
	{5, "5.png"},
	{6, "6.png"},
	{7, "7.png"},
	{8, "8.png"},
	{9, "9.png"},
	{10, "10.png"},
};
class Animation
{
public:
	int framov;
	float speed, currentf;
	bool isplay, flip, loop;
	std::string File;
	Texture t;
	Sprite sprite;
	Animation()
	{
		isplay = true;
		flip = false;
		bool lop = true;
		currentf = 0;
	}
	Animation(std::string file, float speeed, int frameov)
	{
		t.loadFromFile(file + numList[0]);
		speed = speeed;
		framov = frameov;
		File = file;
		currentf = 0;
		flip = false;
	}
	void tik(float time)
	{
		if (!isplay)return;
		currentf += speed * time;
		if (currentf > framov)currentf = 0;
		int i = currentf; std::string filee = File + numList[i];
		t.loadFromFile(File + numList[i]);
		if (flip)t.loadFromFile(File + mirror + numList[i]);;
		sprite.setTexture(t);


	}
};
class AnimManager
{
public:
	std::string currentAnim;
	std::map<std::string, Animation> animList;

	void create(std::string name, std::string file, float speeed, int frameov)
	{
		Animation f(file, speeed, frameov);
		animList[name] = f;
		currentAnim = name;
	}
	void tick(float time) { animList[currentAnim].tik(time); }

	void set(std::string name)
	{
		currentAnim = name;
		animList[currentAnim].flip = 0;
	}

	void draw(RenderWindow &window, int x = 0, int y = 0, int sx = 0, int sy = 0,  bool rect = false)
	{
		if (rect == true) 
		{
			sf::RectangleShape r(sf::Vector2f(sx, sy)); 
			r.setPosition(x, y);
			window.draw(r);	 
		}
		animList[currentAnim].sprite.setOrigin(27, 27);
		animList[currentAnim].sprite.setPosition(x, y);
		window.draw(animList[currentAnim].sprite);
	}

	void flip(bool b = 1) { animList[currentAnim].flip = b; }

	void pause() { animList[currentAnim].isplay = false; }

	void play() { animList[currentAnim].isplay = true; }

	void play(std::string name) { animList[name].isplay = true; }

	bool isPlaying() { return animList[currentAnim].isplay; }

};

#endif
