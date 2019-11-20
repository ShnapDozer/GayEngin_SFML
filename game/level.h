#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "TinyXML/tinyxml.h"
#include "Common.h"

#include <Box2D/Box2D.h>


class Object
{
public:
	std::string name;
	FloatRect rect;
	Object(std::string n, FloatRect r)
	{
		name = n;
		rect = r;
	}

};

class BodyInf 
{
public:
	b2PolygonShape PolygonShape;
	b2CircleShape PolygonCircle;
	b2BodyDef BodyDef;
	b2FixtureDef FixtureDef;
	b2Body* Body;
	std::string name;
	BodyInf() {};
	BodyInf(std::string name,b2PolygonShape &PolygonShape, b2BodyDef &BodyDef, b2FixtureDef &FixtureDef, b2Body* &Body)
	{
		this->name = name;
		this->PolygonShape = PolygonShape;
		this->BodyDef = BodyDef;
		this->FixtureDef = FixtureDef;
		this->Body = Body;
	}
	BodyInf(std::string name, b2PolygonShape &PolygonShape, b2BodyDef &BodyDef, b2Body* &Body)
	{
		this->name = name;
		this->PolygonShape = PolygonShape;
		this->BodyDef = BodyDef;
		this->Body = Body;
	}
	BodyInf(std::string name, b2CircleShape &circle , b2BodyDef &BodyDef, b2Body* &Body)
	{
		this->name = name;
		this->PolygonCircle = circle;
		this->BodyDef = BodyDef;
		this->Body = Body;
	}
};

class Lvl {
private:

	std::string FileRect;
	std::string FileBackG;

	Texture t;
	Sprite Lev;

public:

	std::vector <Object> Obje;
	std::vector <BodyInf> WallInf;
	FloatRect size;

	Lvl(std::string FileRect, std::string FileBackG)//, std::vector <Object> &obj)
	{
		this->FileRect = FileRect;
		this->FileBackG = FileBackG;
		//this->Obje = obj;
	}

	void Load()
	{
		FloatRect SizLev;
		std::string nameobj;

		t.loadFromFile(FileBackG);
		Lev.setTexture(t);
		Lev.setPosition(0, 0);
		SizLev=Lev.getGlobalBounds();

		TiXmlDocument doc(FileRect.c_str());
		if (!doc.LoadFile())
		{
			std::cout << "XML XYU :(" << std::endl;
			return;
		}
		TiXmlElement *element = doc.FirstChildElement("Name");
		TiXmlElement *objectel;
		objectel = element->FirstChildElement("Rect");
		while (objectel)
		{
			if (objectel->Attribute("name") != NULL)
			{
				nameobj = objectel->Attribute("name");
			}
			if (objectel->Attribute("X") != NULL)
			{
				size.left = atoi(objectel->Attribute("X"));
			}
			if (objectel->Attribute("Y") != NULL)
			{
				size.top = atoi(objectel->Attribute("Y"));
			}
			if (objectel->Attribute("W") != NULL)
			{
				size.width = atoi(objectel->Attribute("W"));
			}
			if (objectel->Attribute("H") != NULL)
			{
				size.height = atoi(objectel->Attribute("H"));
			}

			size.left   = size.left / SCALE + size.width / 2 / SCALE;
			size.top    = size.top / SCALE + size.height / 2 / SCALE;
			size.width  = size.width  / 2 / SCALE;
			size.height = size.height / 2 / SCALE;

			Object o(nameobj, size);
			Obje.push_back(o);
			objectel = objectel->NextSiblingElement("Rect");
		}

	}

	void Draw(RenderWindow &window)
	{
		window.draw(Lev);
	}

	Object getObj(std::string name)
	{
		for (int i = 0; i < Obje.size(); i++)
		{
			if (Obje[i].name == name) return Obje[i];
		}
		Object EmtyObj(name, FloatRect(100, 100, 0, 0));
		std::cout << "Object " << name << " is not found! :(" << std::endl;
		return EmtyObj;
	}

	void SetWall(b2World &World)
	{
		for (int i = 0; i < Obje.size(); i++) 
		{
			if (Obje[i].name=="Solid")
			{

				b2PolygonShape gr;
				gr.SetAsBox(Obje[i].rect.width, Obje[i].rect.height);

				b2BodyDef bdef;

				bdef.type = b2_staticBody;
				bdef.position.Set(Obje[i].rect.left , Obje[i].rect.top );

				b2FixtureDef GrFix;

				GrFix.shape = &gr;
				if (Obje[i].rect.width > Obje[i].rect.height) { GrFix.friction = 100; }
				else GrFix.friction = 0;
	
 
				b2Body* b_ground = World.CreateBody(&bdef);
				b_ground->CreateFixture(&GrFix);

				BodyInf Wall("Solid", gr, bdef, GrFix, b_ground);

				WallInf.push_back(Wall);
			}
		}
	}

	std::vector <Object> GetAllObjects() { return Obje; }
	std::vector <BodyInf> GetAllWallInf() { return WallInf; }
};



//prepare a shape definition


#endif
