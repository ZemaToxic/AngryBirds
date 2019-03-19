#include "Box2DObject.h"


CBox2DObject::CBox2DObject(b2World *_world, ColliderShape _colliderShape, b2FixtureDef& _fixtureDef, bool _isDynamic, std::string ObjTexture, b2Vec2 _initPos, b2Vec2 _initSize)
	: CObject(CProgrammerManager::GetInstance().GetProgram(DEFAULT), ObjTexture, MESH_2D_SPRITE),
	m_world(_world)
{

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = false;
	bodyDef.position = _initPos;
	m_objPosition = { _initPos.x, _initPos.y, 0.0f };

	if (_isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}

	//Body gets created using above body definition, which defines the initial position and if the body is dynamic
	//A world is required, made using

	//	b2Vec2 gravity(0.0f, -200.0f);  //
	//  m_world = new b2World(gravity); //

	m_body = m_world->CreateBody(&bodyDef);

	//A fixture is required for box 2d to make physics equations with, made using
	/*
	b2FixtureDef fixtureDef; 
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f; 
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; Bouncyness
	*/

	//A shape is required for the fixture
	b2PolygonShape boxShape;
	b2CircleShape circleShape;

	switch (_colliderShape)
	{
	case BOX:
	{
		boxShape.SetAsBox(_initSize.x, _initSize.y); //The shape defined for the fixture also defines size of the object
		_fixtureDef.shape = &boxShape;
	}
		break;
	case CIRCLE:
	{
		circleShape.m_radius = _initSize.x / 2;		//The shape defined for the fixture also defines size of the object
		_fixtureDef.shape = &circleShape;
	}
		break;
	default:
		break;
	}

	m_objScale = { _initSize.x, _initSize.y, 1.0f };
	m_body->CreateFixture(&_fixtureDef); //The fixture gets attached to the body
	m_body->SetUserData(this);
}

void CBox2DObject::Process()
{
	m_objPosition = glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 0.0f); //The opengl object gets given the position calculated by box2d
	Rotation(glm::degrees(m_body->GetAngle()));

	/* At some point in process you must tell the world when to step, or the timings for physics equations
	float32 timeStep = 1.0f / 120.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	m_world->Step(timeStep, velocityIterations, positionIterations);
	*/
}

void CBox2DObject::SetPos(b2Vec2 NewPos)
{
	m_body->SetTransform(NewPos, m_body->GetAngle());
	m_objPosition = glm::vec3( NewPos.x, NewPos.y, 0.0f );
}

