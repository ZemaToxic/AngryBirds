#pragma once
#include "utils.h"
#include "GameModel.h"

enum collider_shape
{
	BOX,
	CIRCLE
};

enum objType
{
	scenery = 0x0001,
	player = 0x0002,
	obstacle = 0x0004,
	enemy = 0x0008,
	wall = 0x0016
};


//Derivitave of object class
class box2D : public GameModel
{
public:
	box2D(b2World* _world, ModelType modelType, objType _objType, b2FixtureDef& _fixtureDef, bool _isDynamic,
	      std::string ObjTexture, Camera* camera, GLuint program,
	      b2Vec2 _initPos = {utils::window_width / 2, utils::window_height / 2}, b2Vec2 _initSize = {25.0f, 25.0f});
	void process();
	void apply_damage();
	void check_collision();
	int getObjType() const;

	b2Body* get_body() { return (m_body); }

	void set_pos(b2Vec2 NewPos);

	b2Vec2 get_pos() { return (m_body->GetPosition()); }

	bool m_contacting = false;
	bool contact = false;

	void startContact() { m_contacting = true; }

	void endContact()
	{
		m_contacting = false;
		contact = false;
	}

	~box2D();
private:

	//The definitions for the world and body
	b2World* m_world;
	b2Body* m_body;
	objType m_obj;
	int health;
};
