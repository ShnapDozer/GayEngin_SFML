#ifndef Physi_cpp
#define Physi_cpp


#include <iostream>
#include <String>
#include <vector>
#include <cmath>

#include "level.h"
#include "Common.h"

#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

using namespace sf;


class Physic
{

public:
	FloatRect SIZE;
	
	
	bool onGround = false;

	b2PolygonShape shape;
	b2CircleShape circle;

	b2BodyDef bdef;
	b2Body* playBody;
	BodyInf Play;




	Physic() {}


	Physic(FloatRect SIZE, Lvl &lvl, b2World &World)
	{

		

		onGround = false;
		this->SIZE = SIZE;



		bdef.type = b2_dynamicBody;
		bdef.fixedRotation = true;

		shape.SetAsBox(ScalCent(SIZE.width), ScalCent(SIZE.height));
		bdef.position.Set((SIZE.left + SIZE.width / 2) / SCALE, (SIZE.top - SIZE.height / 2) / SCALE);

		playBody = World.CreateBody(&bdef);
		playBody->CreateFixture(&shape, 1);

		BodyInf Play("Plaer", shape, bdef, playBody);
		
	}

	BodyInf GetPlayInf() { return Play; }
	

	virtual void update(b2World &World)
	{
		b2Vec2 posG = playBody->GetPosition();
		posG.y += (SIZE.height/2+10) / SCALE;
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
			{
				if (f->TestPoint(posG))  onGround = true;
			}
	}

	FloatRect GetR()
	{
		FloatRect r{ bdef.position.x,bdef.position.y,SIZE.width,SIZE.height };
		return r;
	}


};

class PhysicIt : public Physic
{
protected:
	std::string type;
public:

	PhysicIt(FloatRect SIZE, b2World &World, std::string type,float height) 
	{
		
		this->SIZE = SIZE;

		bdef.type = b2_dynamicBody;
		bdef.fixedRotation = false;

		if (type == "Box") { shape.SetAsBox(SIZE.width / 2 / SCALE, SIZE.height / 2 / SCALE); }
		if (type == "Circle") { circle.m_radius = SIZE.width / SCALE; }

		bdef.position.Set(SIZE.left / SCALE, SIZE.top / SCALE);
		playBody = World.CreateBody(&bdef);

		if (type == "Box") { playBody->CreateFixture(&shape, height); BodyInf Play("Box", shape, bdef, playBody); }
		if (type == "Circle") { playBody->CreateFixture(&circle, height); BodyInf Play("Circle", circle, bdef, playBody);}

	}

	void update() 
	{
		
	}

};

 



#endif