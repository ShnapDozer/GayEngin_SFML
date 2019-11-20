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
	b2FixtureDef BoxFix;
	BodyInf Play;




	Physic() {}


	Physic(FloatRect SIZE, Lvl &lvl, b2World &World)
	{

		
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
		
		b2Vec2 pos = playBody->GetPosition();
		pos.y += (SIZE.height/2 +1) / SCALE;
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
		     if (f->TestPoint(pos))  onGround = true;
			
	}

	FloatRect GetPosition()
	{
		FloatRect r{ bdef.position.x * SCALE,bdef.position.y * SCALE,SIZE.width * SCALE, SIZE.height * SCALE };
		return r;
	}



};

class PhysicIt : public Physic
{
protected:
	std::string type;
public:

	PhysicIt(std::string type, FloatRect SIZE, b2World &World,float height)
	{
		
		this->SIZE = SIZE;

		bdef.type = b2_dynamicBody;
		bdef.fixedRotation = false;

		if (type == "Box") 
		{ 
			shape.SetAsBox(SIZE.width / 2 / SCALE, SIZE.height / 2 / SCALE);

			BoxFix.shape = &shape;
			BoxFix.friction = 10;
		}
		if (type == "Circle") { circle.m_radius = SIZE.width / SCALE; }


		bdef.position.Set(SIZE.left / SCALE, SIZE.top / SCALE);
		playBody = World.CreateBody(&bdef);


		if (type == "Box") { playBody->CreateFixture(&BoxFix);  Play = { "Box", shape, bdef, playBody }; }
		if (type == "Circle") { playBody->CreateFixture(&circle, height);  Play = { "Circle", circle, bdef, playBody }; }

	}

};

 



#endif