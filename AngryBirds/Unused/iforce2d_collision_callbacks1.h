#ifndef IFORCE2D_COLLISION_CALLBACKS1_H
#define IFORCE2D_COLLISION_CALLBACKS1_H

#include <vector>
#include <typeinfo>
#include <stdio.h>
#include "../Header/utils.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

//This is just to avoid conflicting names in other tutorial source files.
#define Ball COLLISION_CALLBACKS1_Ball
#define MyContactListener COLLISION_CALLBACKS1_MyContactListener

class Ball {
public:
    //class member variables
    b2Body* m_body;
    float m_radius;
    int m_numContacts;

public:
    Ball(b2World* world, float radius) {
        m_body = NULL;
        m_radius = radius;

        //set up dynamic body, store in class variable
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.position.Set(0, 20);
        m_body = world->CreateBody(&myBodyDef);

        //add circle fixture
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0);
        circleShape.m_radius = m_radius; //use class variable
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape;
        myFixtureDef.density = 1;
        m_body->CreateFixture(&myFixtureDef);

        //set the user data of the body to point to this class
        m_body->SetUserData(this);
        m_numContacts = 0;
    }
    ~Ball() {}

    void startContact() { m_numContacts++; }
    void endContact() { m_numContacts--; }

    void renderAtBodyPosition() {
        //safety check
        if ( m_body == NULL ) return;

        //get current position from Box2D
        b2Vec2 pos = m_body->GetPosition();
        float angle = m_body->GetAngle();

        //call normal render at different position/rotation
        glPushMatrix();
        glTranslatef( pos.x, pos.y, 0 );
        glRotatef( angle * RADTODEG, 0, 0, 1 );
        glScalef( m_radius, m_radius, 1 );
        render();
        glPopMatrix();
    }

    void render() {

        //set color depending on whether currently touching anything
        if ( m_numContacts > 0 )
            glColor3f(1,0,0);//red
        else
            glColor3f(1,1,1);//white

        //nose and eyes
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex2f( 0, 0 );
        glVertex2f(-0.5, 0.5 );
        glVertex2f( 0.5, 0.5 );
        glEnd();

        //mouth
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5,  -0.5 );
        glVertex2f(-0.16, -0.6 );
        glVertex2f( 0.16, -0.6 );
        glVertex2f( 0.5,  -0.5 );
        glEnd();

        //circle
        glBegin(GL_LINE_LOOP);
        for (float a = 0; a < 360 * DEGTORAD; a += 30 * DEGTORAD)
            glVertex2f( sinf(a), cosf(a) );
        glEnd();
    }
};

//contact listener implementation
class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {

        //check if fixture A was a ball
        void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if ( bodyUserData )
            static_cast<Ball*>( bodyUserData )->startContact();

        //check if fixture B was a ball
        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if ( bodyUserData )
            static_cast<Ball*>( bodyUserData )->startContact();

    }

    void EndContact(b2Contact* contact) {

        //check if fixture A was a ball
        void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
        if ( bodyUserData )
            static_cast<Ball*>( bodyUserData )->endContact();

        //check if fixture B was a ball
        bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
        if ( bodyUserData )
            static_cast<Ball*>( bodyUserData )->endContact();

    }
};

MyContactListener contactListenerInstance;

class iforce2d_collision_callbacks1 : public Test
{
public:

    iforce2d_collision_callbacks1() {
        //a static body
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);
        b2Body* staticBody = m_world->CreateBody(&myBodyDef);

        //shape definition
        b2PolygonShape polygonShape;

        //fixture definition
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;

        //add four walls to the static body
        polygonShape.SetAsBox( 20, 1, b2Vec2(0, 0), 0);//ground
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 20, 1, b2Vec2(0, 40), 0);//ceiling
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 1, 20, b2Vec2(-20, 20), 0);//left wall
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox( 1, 20, b2Vec2(20, 20), 0);//right wall
        staticBody->CreateFixture(&myFixtureDef);

        //add one ball entity to scene
        Ball* ball = new Ball(m_world, 1);
        m_balls.push_back( ball );

        //set contact listener in world
        m_world->SetContactListener(&contactListenerInstance);
    }

    ~iforce2d_collision_callbacks1()
    {
        for (int i = 0; i < m_balls.size(); i++)
            delete m_balls[i];
    }

    void Step(Settings* settings)
    {
        //run the default physics and rendering
        Test::Step(settings);

        //render balls
        for (int i = 0; i < m_balls.size(); i++)
            m_balls[i]->renderAtBodyPosition();
    }

    static Test* Create()
    {
        return new iforce2d_collision_callbacks1;
    }

    //class member variables
    std::vector<Ball*> m_balls;

};

//This is just to avoid conflicting names in other tutorial source files.
#undef Ball
#undef MyContactListener

#endif
