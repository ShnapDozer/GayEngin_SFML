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
protected:
	b2Vec2 center;
public:
	FloatRect SIZE;

	std::vector <b2Body*> BullList;
	
	bool onGround = false;

	b2PolygonShape shape;
	b2CircleShape circle;

	b2BodyDef bdef;
	b2Body* playBody;
	b2FixtureDef BoxFix;

	b2Vec2 pos;
	float angle;

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
	

	virtual void update(b2World &World)
	{
		
		pos = playBody->GetPosition();
		pos.y += (SIZE.height/2 +1) / SCALE;
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
				if (f->TestPoint(pos))  onGround = true;
		pos   = playBody->GetPosition();
		angle = playBody->GetAngle();
			
	}

	virtual void shut(Vector2f MapPos, b2World &World, float blastPower = 100, int numBul = 1, float angAtac = 0 )
	{
		center = playBody->GetPosition();

		for (int i = 0; i < numBul; i++)
		{
			float angle = 0;
			//float angle = (i / (float)numRays) * 360 * DEG;
			b2Vec2 rayDir(sinf(angle), cosf(angle));

			b2BodyDef bd;
			bd.bullet = true;
			bd.type = b2_dynamicBody;
			bd.fixedRotation = true; // Вращение необязательное
			bd.linearDamping = 10;
			bd.gravityScale = 0; // Игнорирвать гравитацию
			bd.position = center; // Начальная точка в центре взрыва
			bd.linearVelocity = blastPower * rayDir;
			b2Body* body = World.CreateBody(&bd);

			BullList.push_back(body);

			b2CircleShape circleShape;
			circleShape.m_radius = 0.5; // Очень маленький радиус для тела

			b2FixtureDef fd;
			fd.shape = &circleShape;
			fd.density = 60 / (float)numBul;
			fd.friction = 0; // Трение необязательно
			fd.restitution = 0; // Отражение от тел
			fd.filter.groupIndex = -1; // Частицы не должны сталкиваться друг с другом
			body->CreateFixture(&fd);

		}
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

		if (type == "Box") { playBody->CreateFixture(&BoxFix); }
		if (type == "Circle") { playBody->CreateFixture(&circle, height); }
	}

	void update()
	{
		pos   = playBody->GetPosition();
		angle = playBody->GetAngle();
	}

	void Boom(float blastPower, int numRays, b2World &World)
	{
		center = playBody->GetPosition();


		for (int i = 0; i < numRays; i++)
		{
			float angle = (i / (float)numRays) * 360 * DEG;
			b2Vec2 rayDir(sinf(angle), cosf(angle));

			b2BodyDef bd;
			bd.bullet = true;
			bd.type = b2_dynamicBody;
			bd.fixedRotation = true; // Вращение необязательное
			bd.linearDamping = 10;
			bd.gravityScale = 0; // Игнорирвать гравитацию
			bd.position = center; // Начальная точка в центре взрыва
			bd.linearVelocity = blastPower * rayDir;
			b2Body* body = World.CreateBody(&bd);

			BullList.push_back(body);

			b2CircleShape circleShape;
			circleShape.m_radius = 0.5; // Очень маленький радиус для тела

			b2FixtureDef fd;
			fd.shape = &circleShape;
			fd.density = 60 / (float)numRays;
			fd.friction = 0; // Трение необязательно
			fd.restitution = 0.99f; // Отражение от тел
			fd.filter.groupIndex = -1; // Частицы не должны сталкиваться друг с другом
			body->CreateFixture(&fd);

		}

		
	}

};

 



#endif