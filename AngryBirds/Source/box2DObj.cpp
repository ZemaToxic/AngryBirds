#include "../Header/box2DObj.h"


box2D::box2D(b2World* _world, collider_shape _colliderShape, ModelType modelType, b2FixtureDef& _fixtureDef,
             bool _isDynamic,
             std::string ObjTexture, Camera* camera, Light* light, b2Vec2 _initPos, b2Vec2 _initSize) :
	GameModel(modelType, camera, ObjTexture, light, utils::ambientStrength, utils::specularStrength), m_world(_world)
{
	b2BodyDef body_def;
	body_def.fixedRotation = false;
	body_def.position.Set(_initPos.x, _initPos.y);
	setPosition(glm::vec3{_initPos.x, _initPos.y, 0.0f});

	if (_isDynamic)
	{
		body_def.type = b2_dynamicBody;
	}

	m_body = m_world->CreateBody(&body_def);

	//A shape is required for the fixture
	b2PolygonShape box_shape;
	b2CircleShape circle_shape;

	switch (_colliderShape)
	{
	case BOX:
		{
			box_shape.SetAsBox(_initSize.x, _initSize.y);
			//The shape defined for the fixture also defines size of the object
			_fixtureDef.shape = &box_shape;
		}
		break;
	case CIRCLE:
		{
			circle_shape.m_radius = _initSize.x / 2;
			//The shape defined for the fixture also defines size of the object
			_fixtureDef.shape = &circle_shape;
		}
		break;
	default:
		break;
	}

	setScale(glm::vec3{_initSize.x, _initSize.y, 1.0f});
	m_body->CreateFixture(&_fixtureDef); //The fixture gets attached to the body
	m_body->SetUserData(this);
}

void box2D::process()
{
	setPosition(glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 1.0f));

	//printf("BOX 2D position: %f, %f \n", m_body->GetPosition().x, m_body->GetPosition().y);

	//The openGL object gets given the position calculated by box2d
	//setRotation(glm::degrees(m_body->GetAngle()));
}

void box2D::set_pos(b2Vec2 NewPos)
{
	m_body->SetTransform(NewPos, m_body->GetAngle());
	setPosition(glm::vec3(NewPos.x, NewPos.y, 1.0f));
}
