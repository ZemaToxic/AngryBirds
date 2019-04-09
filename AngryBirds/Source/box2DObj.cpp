#include "../Header/box2DObj.h"

box2D::box2D(b2World* _world, ModelType modelType, objType _objType, b2FixtureDef& _fixtureDef, bool _isDynamic, std::string ObjTexture, Camera* camera, GLuint program, b2Vec2 _initPos, b2Vec2 _initSize) : GameModel(modelType, camera, ObjTexture), m_world(_world)
{
	setProgram(program);
	b2BodyDef body_def;
	if (_isDynamic) { body_def.type = b2_dynamicBody; }
	body_def.fixedRotation = false;
	body_def.position.Set(_initPos.x, _initPos.y);
	setPosition(glm::vec3{_initPos.x, _initPos.y, 0.0f});
	m_body = m_world->CreateBody(&body_def);

	//A shape is required for the fixture
	b2PolygonShape box_shape;
	b2CircleShape circle_shape;
	switch (_objType) {
	case player:
	{
		m_obj = _objType;
		circle_shape.m_radius = _initSize.x;
		_fixtureDef.shape = &circle_shape;
		_fixtureDef.filter.categoryBits = player;
		_fixtureDef.filter.maskBits = scenery | obstacle | enemy | player;
		health = 5;
	}
	break;
	case scenery:
	{
		m_obj = _objType;
		box_shape.SetAsBox(_initSize.x, _initSize.y);
		_fixtureDef.shape = &box_shape;
		_fixtureDef.filter.categoryBits = scenery;
		_fixtureDef.filter.maskBits = player | scenery | obstacle | enemy;
		health = -1;
	}
	break;
	case obstacle:
	{
		m_obj = _objType;
		box_shape.SetAsBox(_initSize.x, _initSize.y);
		_fixtureDef.shape = &box_shape;
		_fixtureDef.filter.categoryBits = obstacle;
		_fixtureDef.filter.maskBits = player | scenery | enemy | obstacle;
		health = 3;
	}
	break;
	case enemy:
	{
		m_obj = _objType;
		circle_shape.m_radius = _initSize.x;
		_fixtureDef.shape = &circle_shape;
		_fixtureDef.filter.categoryBits = enemy;
		_fixtureDef.filter.maskBits = player | scenery | enemy | obstacle;
		health = 2;
	}
	break;
	case wall:
	{
		m_obj = _objType;
		box_shape.SetAsBox(_initSize.x, _initSize.y);
		_fixtureDef.shape = &box_shape;
		_fixtureDef.filter.categoryBits = wall;
		_fixtureDef.filter.maskBits = player | scenery | obstacle | enemy;
		health = -1;
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
	if (health == 0) { m_body->SetActive(false); }
	else {
		setPosition(glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 1.0f));
		rotate({ 0, 0, m_body->GetAngle()});
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
	if (health <= -1) return;
	health = health - 1;
	printf("Obj Health = %d\n", health);
}

void box2D::check_collision()
{
	if (m_obj != scenery) {
		if (!contact) {
			if (m_contacting) {
				contact = true;
				apply_damage();
			}
		}
		if (!m_contacting) { contact = false; }
	}
}

int box2D::getObjType() const { return m_obj; }

box2D::~box2D() { m_body->GetWorld()->DestroyBody(m_body); }
