
// GeyEngine v0.1

/*======================================

   To include include imgui go to => "Project" => "Katalogs VC++" => "Include katalogs"
   and set way to "game\ImGui\imgui-master" and "game\ImGui\imgui-sfml-master" IT ONLY 32 BIT!!!!

======================================*/


#ifndef Sourse_h
#define Sourse_h


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <String>
#include <vector>
#include <Windows.h>

#include "People.h"
#include "Anim.h"
#include "level.h"
#include "UI.h"
#include "Physics.h"
#include "Common.h"

#include "TinyXML/tinyxml.h"

#include "imgui.h"
#include "imgui-sfml.h"

#include <Box2D/Box2D.h>


View view;
using namespace sf;


namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}


void viewset(RenderWindow &window, b2Vec2 pos,int maxx,int maxy)
{
	pos.x = pos.x*SCALE;
	pos.y = pos.y*SCALE;
	window.setView(view);
	if (pos.y  <= maxy/2) { pos.y = maxy / 2; }
	if (pos.y  >= 150*16-344) { pos.y = 150 * 16 - 360; }
	if (pos.x >= 300 * 16 - 624) { pos.x = 300 * 16 - 640; }
	if (pos.x  <= maxx/2) { pos.x = maxx / 2; }
	view.setCenter(pos.x, pos.y);
}

void Hide()
{
	HWND Hide;
	AllocConsole();
	Hide = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Hide, 0);
}

void BullClear(std::vector <b2Body*> &BullList, b2World &World)
{
	for (int i = 0; i < BullList.size(); i++)
	{
		World.DestroyBody(BullList[i]);
	}
	BullList.clear();
}



bool GameStart()
{

	//Настройки:

	bool rect = false;
	bool ImgCons = true;

	//Hide();   // ----------- Скрывает консоль

	const int x = 1280;
	const int y = 720;

	float Dx = 1000;
	float Dy = 1000;

	const ImVec2 ConsMenuSize(300, y);
	const ImVec2 ConsMenuPos(x - 300, 0);

	b2Vec2 Gravity(0.f, 14.0f);

	//-----------------------------------------

	bool inventor = false;

	

	b2World World(Gravity);

	RectangleShape rectw(sf::Vector2f(5, 5));
	rectw.setFillColor(Color(125, 185, 163));
	rectw.setOrigin(rectw.getSize().x / 2, rectw.getSize().y / 2);




	RenderWindow window(/*VideoMode::getFullscreenModes()[0]*/VideoMode(x, y), "GeyEngine v0.1");

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	ImGui::SFML::Init(window);
	

	view.reset(FloatRect(0, 0, 1280, 720));

	//Level
	Lvl level("Res/Map/test.xml", "Res/BackGround/Background.png");
	level.Load();
	level.SetWall(World);

	
	//Animation
	AnimManager Her;
	Her.create("Run", "Res/Hero/H", 0.003, 7);
	Her.create("Stay", "Res/Hero/HS", 0.003, 1);

	AnimManager BoxA;
	BoxA.create("Stay", "Res/Other/Box", 0.003, 1);


	
	//Hero and Entity
	Object player = level.getObj("Play");

	FloatRect sizeH{ player.rect.left, player.rect.top,54,54 };
	FloatRect sizeB{ 1000, 100, 60,62 };

	People h(sizeH);

	Physic HERO(sizeH, level, World);

	PhysicIt Box("Box", sizeB, World, 1);


	//UI
	Inventory i("Res/UI/", inventor);
	i.add(0);
	i.add(1);
	i.add(2);



	Clock Time;
	Clock clock;


	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 300;

		Vector2i locPos = Mouse::getPosition(window);
		Vector2f MapPos = window.mapPixelToCoords(locPos);


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
				ImGui::SFML::Shutdown();
				return false;
			}

			ImGui::SFML::ProcessEvent(event);

			/*if (event.type == sf::Event::LostFocus)
				myGame.pause();

			if (event.type == sf::Event::GainedFocus)
				myGame.resume(); */ // -------- Что то интересное но я хз

			//--------------Клава:


			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code) 
				{
				case sf::Keyboard::B:
					Box.Boom(10000, 100, World);

					break;
				case sf::Keyboard::R:
					return true;

					break;
				case sf::Keyboard::E:
					if (inventor) inventor = false;
					else inventor = true;

					break;
				case sf::Keyboard::C:
					BullClear(Box.BullList, World);

					break;
				}
			}
		}

		//----------------------Box2d

		World.Step(1.0f / 60.f, 8, 3);
		b2Vec2 a = HERO.playBody->GetLinearVelocity();

		//----------------------Клава:

		if (HERO.onGround) { Dx = 1000; }
		else { Dx = 500; }
		if (Keyboard::isKeyPressed(Keyboard::A)) { if (a.x > -20) { HERO.playBody->ApplyForceToCenter(b2Vec2(-Dx, 0), 1); }  Her.set("Run"); }
		if (Keyboard::isKeyPressed(Keyboard::D)) { if (a.x < 20) 
		{ 
			HERO.playBody->ApplyForceToCenter(b2Vec2(Dx, 0), 1); }  Her.set("Run"); 
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (HERO.onGround == true && a.y > -5) { HERO.playBody->ApplyForceToCenter(b2Vec2(0, -Dy), 0); Her.set("Run"); HERO.onGround = false;}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			if (inventor == true) { inventor = false; }
		}
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0) { view.zoom(0.75); }
			if (event.mouseWheelScroll.delta < 0) { view.zoom(1.35); }
		}

		

		//----------------------Колесо

		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();
		event.mouseWheelScroll.delta = 0;

		//----------------------Imgui

		
		ImGui::SFML::Update(window, Time.restart());
		
		if (ImgCons) 
		{
			ImGui::Begin("Console");

			ImGui::SetWindowSize(ConsMenuSize);
			ImGui::SetWindowPos(ConsMenuPos);

			if (ImGui::Button("RectPaint")) { if (rect)rect = false; else rect = true; }

			
			
			ImGui::Value("y", HERO.pos.y);
			ImGui::Value("x", HERO.pos.x);
			ImGui::Value("What", HERO.onGround);

			ImGui::SliderFloat("Dx", &Dx, 0, 1000);
			ImGui::SliderFloat("Dy", &Dy, 0,1000 );

			ImGui::End();
		}
	

		//----------------------Start Draw
		window.clear(Color(77, 83, 140));

		level.Draw(window);

		

		if (rect && !Box.BullList.empty())
		{
			for (int i = 0; i < Box.BullList.size(); i++)
			{
				b2Vec2 posW = Box.BullList[i]->GetPosition();
				rectw.setPosition(posW.x*SCALE, posW.y*SCALE);
				window.draw(rectw);
			}
		}


		i.update(MapPos, view.getCenter().x, view.getCenter().y, x, y);
		
		HERO.update(World);
		Box.update();
		
		Her.tick(time);
		BoxA.tick(time);

		if (a.x <  15 && a.x >=0 ) Her.set("Stay");
		if (a.x > -15 && a.x <= 0) Her.set("Stay");



		viewset(window, HERO.pos, x, y);

		if (inventor == true)
		{
			i.draw(window);
		}

		Her.draw  (window, HERO.pos);
		BoxA.draw (window, Box.pos, Box.angle);


		ImGui::SFML::Render(window);


		window.display();

	}
}

void GameRun() 
{
	if (GameStart()) { GameRun(); }
}

int main()
{
	GameRun();
	return 0;
}






#endif