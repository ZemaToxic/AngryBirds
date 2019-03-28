#pragma once

#include  "utils.h"
#include "../Dependencies/Box2D/Box2D.h"
#include "box2DObj.h"

class Contact : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override
	{
		//box2D* objA;
		//check if fixture A was a box2D obj
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData)
			static_cast<box2D*>(bodyUserData)->startContact();

		box2D* objB;
		//check if fixture B was a box2D obj
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData)
		{
			objB = static_cast<box2D*>(bodyUserData);
			b2Vec2 velB = objB->get_body()->GetLinearVelocity();
			glm::vec3 vecB = { velB.x, velB.y, 0.0f };
			float speedB = glm::length(vecB);

			if (objB->get_body()->GetType() == b2_dynamicBody)
			{
				if (speedB > upper)
				{
					// Damage 
					static_cast<box2D*>(bodyUserData)->startContact();
				}
				else if (speedB < lower)
				{
					//printf("Speed: %f \n", speedB);
				}
			}
		}
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

private:
	const int upper = 50;
	const int lower = 20;
};
