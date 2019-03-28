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
	player,
	scenery,
	obstacle,
	enemy
};


//Derivitave of object class
class box2D : public GameModel
{
public:

	box2D(b2World* _world, collider_shape _colliderShape, ModelType modelType, objType _objType, b2FixtureDef& _fixtureDef, bool _isDynamic, std::string ObjTexture, Camera* camera, GLuint program, b2Vec2 _initPos = {0.0f, 0.0f}, b2Vec2 _initSize = {1.0f, 1.0f});

	void process();
	b2Body* get_body() { return (m_body); }
	void set_pos(b2Vec2 NewPos);
	b2Vec2 get_pos() { return (m_body->GetPosition()); }
	void apply_damage();
	void check_collision();

	bool m_contacting = false;
	bool contact = false;
	
	void startContact() { m_contacting = true; }
	void endContact() { m_contacting = false; }

	~box2D();

private:

	//The definitions for the world and body
	b2World* m_world;
	b2Body* m_body;
	objType m_obj;
	collider_shape m_Collider;
	int health;
};
