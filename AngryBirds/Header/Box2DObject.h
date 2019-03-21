#pragma once
#ifndef __BOX2DOBJECT_H__
#define __BOX2DOBJECT_H___

#include "utils.h"
#include "GameModel.h"
#include "../Dependencies/Box2D/Box2D.h"

enum ColliderShape
{
	BOX,
	CIRCLE
};

//Derivitave of object class
class CBox2DObject : public GameModel
{
public:

	CBox2DObject(b2World* _world, ColliderShape _colliderShape, ModelType modelType, b2FixtureDef& _fixtureDef, bool _isDynamic,
	             std::string ObjTexture, Camera* camera, Light* light, b2Vec2 _initPos = {0.0f, 0.0f},
	             b2Vec2 _initSize = {1.0f, 1.0f});

	void process();
	b2Body* get_body() { return (m_body); }
	void set_pos(b2Vec2 NewPos);
	b2Vec2 get_pos() { return (m_body->GetPosition()); }

	~CBox2DObject();

private:

	//The definitions for the world and body
	b2World* m_world;
	b2Body* m_body;
	ColliderShape m_Collider;
};
#endif // !__PHYSICSOBJECT_H__
