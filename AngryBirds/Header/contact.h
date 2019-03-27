#pragma once

#include  "utils.h"
#include "../Dependencies/Box2D/Box2D.h"
#include "box2DObj.h"

class Contact : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override
	{
		//check if fixture A was a box2D obj
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		//std::cout << contact << '\n';
		if (bodyUserData)
			static_cast<box2D*>(bodyUserData)->startContact();

		//check if fixture B was a box2D obj
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<box2D*>(bodyUserData)->startContact();
	}

	void EndContact(b2Contact* contact) override
	{
		//check if fixture A was a box2D obj
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<box2D*>(bodyUserData)->endContact();

		//check if fixture B was a box2D obj
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<box2D*>(bodyUserData)->endContact();
	}
};
