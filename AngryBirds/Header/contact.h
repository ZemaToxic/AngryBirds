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
			auto body_user_data = contact->GetFixtureA()->GetBody()->GetUserData();
			if (body_user_data) static_cast<box2D*>(body_user_data)->startContact();
			//check if fixture B was a box2D obj
			body_user_data = contact->GetFixtureB()->GetBody()->GetUserData();
			if (body_user_data) {
				auto* obj_b = static_cast<box2D*>(body_user_data);
				const auto vel_b = obj_b->get_body()->GetLinearVelocity();
				const glm::vec3 vec_b = {vel_b.x, vel_b.y, 0.0f};
				const auto speed_b = length(vec_b);
				if (obj_b->get_body()->GetType() == b2_dynamicBody) {
					if (speed_b > upper) {
						// Damage 
						static_cast<box2D*>(body_user_data)->startContact();
					}
					else if (speed_b < lower) {
						//printf("Speed: %f \n", speedB);
					}
				}
			}
		}

		void EndContact(b2Contact* contact) override
		{
			//check if fixture A was a box2D obj
			auto body_user_data = contact->GetFixtureA()->GetBody()->GetUserData();
			if (body_user_data) static_cast<box2D*>(body_user_data)->endContact();

			//check if fixture B was a box2D obj
			body_user_data = contact->GetFixtureB()->GetBody()->GetUserData();
			if (body_user_data) static_cast<box2D*>(body_user_data)->endContact();
		}

	private:
		const int upper = 20;
		const int lower = 2;
	};
