#include "../Header/Box2DObject.h"


CBox2DObject::CBox2DObject(b2World* _world, ColliderShape _colliderShape, b2FixtureDef& _fixtureDef, bool _isDynamic,
                           std::string ObjTexture, Camera* camera, Light* light, b2Vec2 _initPos, b2Vec2 _initSize)
	: GameModel(kSphere, camera, ObjTexture, light, utils::ambientStrength, utils::specularStrength),
	  m_world(_world)
{
	b2BodyDef bodyDef;
	bodyDef.fixedRotation = false;
	bodyDef.position = _initPos;
	setPosition(glm::vec3{_initPos.x, _initPos.y, 0.0f});

	if (_isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}

	m_body = m_world->CreateBody(&bodyDef);

	//A shape is required for the fixture
	b2PolygonShape boxShape;
	b2CircleShape circleShape;

	switch (_colliderShape)
	{
	case BOX:
		{
			boxShape.SetAsBox(_initSize.x, _initSize.y);
			//The shape defined for the fixture also defines size of the object
			_fixtureDef.shape = &boxShape;
		}
		break;
	case CIRCLE:
		{
			circleShape.m_radius = _initSize.x / 2; //The shape defined for the fixture also defines size of the object
			_fixtureDef.shape = &circleShape;
		}
		break;
	default:
		break;
	}

	scale = {_initSize.x, _initSize.y, 1.0f};
	m_body->CreateFixture(&_fixtureDef); //The fixture gets attached to the body
	m_body->SetUserData(this);
}

void CBox2DObject::Process()
{
	setPosition(glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 1.0f));
	//The opengl object gets given the position calculated by box2d
	//setRotation(glm::degrees(m_body->GetAngle()));
}

void CBox2DObject::SetPos(b2Vec2 NewPos)
{
	m_body->SetTransform(NewPos, m_body->GetAngle());
	setPosition(glm::vec3(NewPos.x, NewPos.y, 1.0f));
}
