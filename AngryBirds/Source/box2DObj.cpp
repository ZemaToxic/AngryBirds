#include "../Header/box2DObj.h"


box2D::box2D(b2World* _world, collider_shape _colliderShape, ModelType modelType, objType _objType,
             b2FixtureDef& _fixtureDef, bool _isDynamic, std::string ObjTexture, Camera* camera, GLuint program,
             b2Vec2 _initPos, b2Vec2 _initSize) : GameModel(modelType, camera, ObjTexture), m_world(_world)
{
	setProgram(program);

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
			//The shape defined for the fixture also defines size of the object
			box_shape.SetAsBox(_initSize.x, _initSize.y);
			_fixtureDef.shape = &box_shape;
		}
		break;
	case CIRCLE:
		{
			//The shape defined for the fixture also defines size of the object
			circle_shape.m_radius = _initSize.x;
			_fixtureDef.shape = &circle_shape;
		}
		break;
	default:
		break;
	}

	switch (_objType)
	{
	case player:
		{
			health = -1;
		}
		break;
	case scenery:
		{
			health = -1;
		}
		break;
	case obstacle:
		{
			health = 25;
		}
		break;
	case enemy:
		{
			health = 25;
		}
		break;
	}
	setScale(glm::vec3{_initSize.x, _initSize.y, 1.0f});
	m_body->CreateFixture(&_fixtureDef); //The fixture gets attached to the body
	m_body->SetUserData(this);
}

void box2D::process()
{
	check_collision();
	//printf("BOX 2D position: %f, %f \n", m_body->GetPosition().x, m_body->GetPosition().y);
	if (health == 0)
	{
		m_body->SetActive(false);
	}
	else
	{
		setPosition(glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 1.0f));
		update();
	}
}

void box2D::set_pos(b2Vec2 NewPos)
{
	m_body->SetTransform(NewPos, m_body->GetAngle());
	setPosition(glm::vec3(NewPos.x, NewPos.y, 1.0f));
}

void box2D::apply_damage()
{
	if (health <= -10) return;

	health = health - 1;
	printf("Obj Health = %d\n", health);
}

void box2D::check_collision()
{
	if (!contact)
	{
		if (m_contacting)
		{
			contact = true;
			apply_damage();
		}
	}
	if (!m_contacting)
	{
		contact = false;
	}
}

box2D::~box2D()
{
	m_body->GetWorld()->DestroyBody(m_body);
}
